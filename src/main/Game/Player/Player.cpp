
#include "Player.hpp"
#include "../../../CustomEventList.hpp"
#include "../../../SivComponent/SivComponent.hpp"
#include "../Bullet.hpp"

using dictype = ExitGames::Common::Dictionary<nByte, double>;

constexpr int PosSyncPerSecond = 8;

Player::Player()
    : syncpos((int32_t)(1000 / PosSyncPerSecond), [&]() { SyncPos(); })
{
    isMine = false;
    // spd = 60.0;
    spd = 60.0 * 5;
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

void Player::Update()
{
    //自分じゃなければ何もしない
    if (!isMine)
    {
        SyncPosWithEasing();
        return;
    }

    //位置同期を行う
    syncpos.Run();

    if (state == PlayerStates::reviving)
    {
        Revive();
        return;
    }

    Move();
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

    ExitGames::Common::Dictionary<nByte, double> dic =
        ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, double> >(eventContent).getDataCopy();

    double x = *dic.getValue(DataName::Player::posX);
    double y = *dic.getValue(DataName::Player::posY);

    //到着先を終点に設定
    targetPos = {x, y};
    // GetGameObject().lock()->SetPosition({x, y});
}

void Player::SendInstantiateMessage()
{
    // 自己位置の送信
    dictype dic;

    auto pos = GetGameObject().lock()->GetPosition();
    dic.put(DataName::Player::posX, pos.x);
    dic.put(DataName::Player::posY, pos.y);

    networkSystem->GetClient().opRaiseEvent(false, dic, CustomEvent::PlayerInit);
}

void Player::SyncPos()
{
    // 自己位置の送信
    dictype dic;

    auto pos = GetGameObject().lock()->GetPosition();
    dic.put(DataName::Player::posX, pos.x);
    dic.put(DataName::Player::posY, pos.y);

    networkSystem->GetClient().opRaiseEvent(true, dic, CustomEvent::PlayerSync);
}

constexpr int offset = 200;
s3d::Vec2 Player::playerInitpos[4] = {s3d::Vec2(-offset, -offset), s3d::Vec2(offset, -offset), s3d::Vec2(-offset, offset), s3d::Vec2(offset, offset)};

//壁とぶつかったときの処理
void Player::OnWall()
{
    s3d::Print(U"OnWall");

    GetGameObject().lock()->SetPosition(movehistory);
}

void Player::OnEnemy() {}
void Player::OnEnemyBullet() {}

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
        auto bullet = other->GetComponent<Bullet>();
        life -= bullet->attack;

        //ライフが尽きたら
        if (life <= 0 && state != PlayerStates::reviving)
        {
            //状態を変更して色を黒くする
            state = PlayerStates::reviving;
            counttimer = 6.0;
            rect->SetColor(defaultcolor.lerp(s3d::Palette::Black, 0.6));
        }
    }
}
