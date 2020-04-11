
#include "Player.hpp"
#include "../../../SivComponent/SivComponent.hpp"
#include "../../CustomEventList.hpp"
#include "../Bullet.hpp"

constexpr int PosSyncPerSecond = 7;

/*
プレイヤーの個性
復活型 HPが低いが復活速度がとても早い　遊撃向き
攻撃型 攻撃力が1.6倍ぐらいある　拠点防衛向き
HP型　HPが3倍ぐらいあり、当たり判定が少し小さい　乱戦に強い
速度型 移動速度と弾速が1.3倍ぐらいある　スナイパー処理向き
*/

Player::Player()
    : syncstatus((int32_t)(1000 / PosSyncPerSecond), [&]() { SyncStatus(); })
{
    //ベース数値を設定する

    spd = 115.0;
    maxlife = 100;
    life = maxlife;
    regenespd = 0;
    reviveCost = 2.5;

    fire.attack = 10;
    fire.lifetime = 1.35;
    fire.spread = 1;
    fire.speed = 180;

    rivivetimer = 0;

    // spd = 60.0 * 5;
}

void Player::SetType(PlayerType _type)
{
    type = _type;

    switch (type)
    {
        case PlayerType::Standard:
            reviveCost = 1.0;
            break;
        case PlayerType::Attack:
            fire.attack = 18;
            fire.speed * 0.85;
            break;
        case PlayerType::Defence:
            maxlife = 800;
            life = maxlife;
            spd *= 0.9;
            break;
        case PlayerType::Speed:
            maxlife = 70;
            life = maxlife;
            spd *= 1.54;
            fire.speed *= 1.8;
            fire.lifetime *= 0.4;
            break;
    }
}

void Player::Start2()
{
    //モードに応じた

    //移動先を自分の現座標に
    targetPos = GetGameObject().lock()->GetPosition();

    // GetComponent
    rect = GetGameObject().lock()->GetComponent<Siv::Rect>();
    camera = GetGameObject().lock()->GetParent().lock()->GetParent().lock();

    rect->SetColor(s3d::ColorF(0.1, 0.9, 0.2, 0.3));

    //移動前座標をセット
    movehistory = GetGameObject().lock()->GetPosition();

    //ライフをセット
    life = maxlife;

    //今の色を保存
    defaultcolor = rect->GetColor();
    SetState(PlayerStates::normal);

    int nr = 0;
    const auto lis = networkSystem->GetPlayerList();

    for (; nr < 4; nr++)
    {
        if (lis[nr]->getNumber() == playerNr)
        {
            break;
        }
    }

    //中心の色
    // constexpr std::array<s3d::Color, 4> colors = {s3d::Color(255, 200, 200), s3d::Color(70, 70, 140), s3d::Color(120, 0, 240), s3d::Color(0, 170, 230)};
    // GetGameObject().lock()->GetComponent<Siv::Circle>()->SetColor(colors[nr]);

    // Standard, Attack, Defence, Speed,
    constexpr std::array<s3d::Color, 4> colors = {s3d::Color(0, 255, 0), s3d::Color(255, 0, 0), s3d::Color(0, 0, 255), s3d::Color(255, 255, 0)};
    GetGameObject().lock()->GetComponent<Siv::Circle>()->SetColor(colors[static_cast<int>(GetType())]);
}

void Player::Move()
{
    //キー入力により移動
    s3d::Vec2 axis;
    axis.x = (s3d::KeyD.pressed() - s3d::KeyA.pressed());
    axis.y = (s3d::KeyS.pressed() - s3d::KeyW.pressed());
    if (axis.length() > 1)
    {
        axis.normalize();
    }

    auto obj = GetGameObject().lock();
    //現在位置を記録
    movehistory = obj->GetPosition();
    //移動
    auto pos = obj->GetPosition();
    pos += axis * spd * s3d::Scene::DeltaTime();
    obj->SetPosition(pos);

    //カメラ中央との距離を取る
    auto diff = obj->GetWorldPosition() - s3d::Scene::CenterF();

    constexpr double frame = 50.0;
    diff.x = diff.x - s3d::Clamp(diff.x, -frame, frame);
    diff.y = diff.y - s3d::Clamp(diff.y, -frame, frame);

    const auto currentPos = camera->GetPosition();
    camera->SetPosition(currentPos - diff);
}

void Player::Revive()
{
    rivivetimer -= s3d::Scene::DeltaTime();
    if (rivivetimer < 0)
    {
        //ライフ回復、状態変更、色を戻す
        life = maxlife;
        SetState(PlayerStates::normal);
        rect->SetColor(defaultcolor);
    }
}

//リジェネ回復
void Player::Regenerate()
{
    //自動再生
    if (life < maxlife)
    {
        regenespd += s3d::Scene::DeltaTime() * 0.3;

        regene += s3d::Scene::DeltaTime();
        if (regenespd > 1)
        {
            regene += s3d::Scene::DeltaTime();
        }

        if (regenespd > 2)
        {
            regene += s3d::Scene::DeltaTime();
        }

        if (regenespd > 5)
        {
            regene += s3d::Scene::DeltaTime();
        }
        if (regenespd > 7)
        {
            regene += s3d::Scene::DeltaTime() * 1.5;
        }

        if (regene > 1)
        {
            regene -= 1.0;
            life += 1;
        }
    }
}

void Player::Update()
{
    //状態に合わせた色変更
    switch (GetState())
    {
        case PlayerStates::normal:
            rect->SetColor(defaultcolor);
            break;

        case PlayerStates::reviving:
            rect->SetColor(defaultcolor.lerp(s3d::Palette::Black, 0.7));
            break;
    }

    //自分じゃなければ何もしない
    if (!isMine)
    {
        SyncPosWithEasing();
        return;
    }

    //位置同期を行う
    syncstatus.Run();

    if (GetState() == PlayerStates::reviving)
    {
        Revive();
        return;
    }

    Move();

    Regenerate();
}

void Player::SyncPosWithEasing()
{
    auto pos = GetGameObject().lock()->GetPosition();
    auto diff = targetPos - pos;

    //ラグを考慮して少し早く
    const auto maxspd = spd * s3d::Scene::DeltaTime() * 1.01;
    //移動速度を調整
    if (diff.length() > maxspd)
    {
        diff.normalize();
        diff *= maxspd;
    }

    //移動
    GetGameObject().lock()->SetPosition(pos + diff);
}

void Player::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::PlayerSync)
    {
        return;
    }

    if (playerNr != this->playerNr)
    {
        return;
    }

    auto dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int> >(eventContent).getDataCopy();

    double x = *dic.getValue(DataName::Player::posX);
    double y = *dic.getValue(DataName::Player::posY);

    life = *dic.getValue(DataName::Player::Life);
    const int s = *dic.getValue(DataName::Player::CurrentState);

    SetState(static_cast<PlayerStates>(s));

    //到着先を終点に設定
    targetPos = {x, y};
    // GetGameObject().lock()->SetPosition({x, y});
}

//プレイヤーの初期生成
void Player::SendInstantiateMessage()
{
    ExitGames::Common::Dictionary<nByte, int> dic;

    auto pos = GetGameObject().lock()->GetPosition();
    dic.put(DataName::Player::posX, pos.x);
    dic.put(DataName::Player::posY, pos.y);
    dic.put(DataName::Player::Spd, spd);
    dic.put(DataName::Player::Type, static_cast<int>(type));

    // PlayerManagerが受け取り、Playerにそのまま流し込む
    networkSystem->GetClient().opRaiseEvent(true, dic, CustomEvent::PlayerInit);
}

void Player::SyncStatus()
{
    // 自己位置の送信
    ExitGames::Common::Dictionary<nByte, int> dic;

    auto pos = GetGameObject().lock()->GetPosition();
    dic.put(DataName::Player::posX, pos.x);
    dic.put(DataName::Player::posY, pos.y);
    dic.put(DataName::Player::Life, life);
    dic.put(DataName::Player::CurrentState, static_cast<int>(GetState()));

    // Playerが受け取る
    networkSystem->GetClient().opRaiseEvent(true, dic, CustomEvent::PlayerSync);
}

constexpr int offset = 200;
s3d::Vec2 Player::playerInitpos[4] = {s3d::Vec2(-offset, -offset), s3d::Vec2(offset, -offset), s3d::Vec2(-offset, offset), s3d::Vec2(offset, offset)};

//壁とぶつかったときの処理
void Player::OnWall()
{
    GetGameObject().lock()->SetPosition(movehistory);
}

void Player::OnEnemy() {}

void Player::OnEnemyBullet(std::shared_ptr<GameObject>& other)
{
    auto bullet = other->GetComponent<Bullet>();
    life -= bullet->attack;

    regenespd = 0;

    //ライフが尽きたら
    if (life <= 0 && GetState() != PlayerStates::reviving)
    {
        //状態を変更して色を黒くする
        SetState(PlayerStates::reviving);
        rivivetimer = reviveCost;

        s3d::AudioAsset(U"PlayerDeath").playOneShot((IsMine() ? 0.3 : 0.15));

        return;
    }

    s3d::AudioAsset(U"PlayerDamage").playOneShot((IsMine() ? 0.3 : 0.15));
}

void Player::OnStayCollision(std::shared_ptr<GameObject>& other)
{
    if (!isMine)
    {
        return;
    }

    if (other->GetTag() == UserDef::Tag::Wall)
    {
        OnWall();
    }

    if (other->GetTag() == UserDef::Tag::EnemyBullet)
    {
        OnEnemyBullet(other);
    }
}

void Player::SetState(PlayerStates _state)
{
    //しんだとき
    if (m_state != PlayerStates::reviving && _state == PlayerStates::reviving)
    {
        s3d::AudioAsset(U"PlayerDeath").playOneShot(0.3);
    }

    //復活した時
    if (m_state == PlayerStates::reviving && _state != PlayerStates::reviving)
    {
        s3d::AudioAsset(U"PlayerRevive").playOneShot(0.3);
    }

    m_state = _state;
}
