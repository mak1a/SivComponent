
#include "Player.hpp"
#include "../../../SivComponent/SivComponent.hpp"
#include "../../CustomEventList.hpp"
#include "../Bullet.hpp"

constexpr int PosSyncPerSecond = 7;

Player::Player()
    : syncstatus((int32_t)(1000 / PosSyncPerSecond), [&]() { SyncStatus(); })
{
    isMine = false;
    spd = 115.0;
    // spd = 60.0 * 5;
}

void Player::Start2()
{
    //移動先を自分の現座標に
    targetPos = GetGameObject().lock()->GetPosition();

    // GetComponent
    rect = GetGameObject().lock()->GetComponent<Siv::Rect>();
    camera = GetGameObject().lock()->GetParent().lock()->GetParent().lock();

    constexpr auto playercolor = s3d::Palette::Limegreen.lerp(s3d::Palette::Lightblue, 0.5);
    constexpr auto othercolor = s3d::Palette::Limegreen.lerp(s3d::Palette::Black, 0.2);

    rect->SetColor(isMine ? playercolor : othercolor);

    //移動前座標をセット
    movehistory = GetGameObject().lock()->GetPosition();

    //ライフをセット
    life = maxlife;

    //今の色を保存
    defaultcolor = rect->GetColor();

    state = PlayerStates::normal;
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

    diff.x = diff.x - s3d::Clamp(diff.x, -100.0, 100.0);
    diff.y = diff.y - s3d::Clamp(diff.y, -100.0, 100.0);

    const auto currentPos = camera->GetPosition();
    camera->SetPosition(currentPos - diff);
}

void Player::Revive()
{
    counttimer -= s3d::Scene::DeltaTime();
    if (counttimer < 0)
    {
        //ライフ回復、状態変更、色を戻す
        life = maxlife;
        state = PlayerStates::normal;
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
    switch (state)
    {
        case PlayerStates::normal:
            rect->SetColor(defaultcolor);
            break;

        case PlayerStates::reviving:
            rect->SetColor(defaultcolor.lerp(s3d::Palette::Black, 0.6));
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

    if (state == PlayerStates::reviving)
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
    state = static_cast<PlayerStates>(s);

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
    // PlayerManagerが受け取る
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
    dic.put(DataName::Player::CurrentState, static_cast<int>(state));

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
    if (life <= 0 && state != PlayerStates::reviving)
    {
        //状態を変更して色を黒くする
        state = PlayerStates::reviving;
        counttimer = 6.0;
    }
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
