
#include "Player.hpp"
#include "../../../CustomEventList.hpp"
#include "../../../Matching.hpp"
// #include "../../../SivComponent/SivComponent.hpp"

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
    targetPos = GetGameObject().lock()->GetPosition();

    constexpr auto playercolor = s3d::Palette::Limegreen.lerp(s3d::Palette::Lightblue, 0.5);
    constexpr auto othercolor = s3d::Palette::Limegreen.lerp(s3d::Palette::Black, 0.2);

    GetGameObject().lock()->GetComponent<Siv::Rect>()->SetColor(isMine ? playercolor : othercolor);

    camera = GetGameObject().lock()->GetParent().lock()->GetParent().lock();

    //移動前座標をセット
    movehistory = GetGameObject().lock()->GetPosition();
}

void Player::Update()
{
    if (!isMine)
    {
        SyncPosWithEasing();
        return;
    }

    //位置同期を行う
    syncpos.Run();

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
    if (other->GetTag() == UserDef::Tag::Wall)
    {
        OnWall();
    }
}

//----------------------------------

//プレイヤーを生成
void PlayerManager::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::PlayerInit)
    {
        return;
    }

    //初期化済みなら弾く
    for (const auto& n : players)
    {
        if (n->playerNr == playerNr)
        {
            return;
        }
    }

    ExitGames::Common::Dictionary<nByte, double> dic =
        ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, double> >(eventContent).getDataCopy();

    double x = *dic.getValue(DataName::Player::posX);
    double y = *dic.getValue(DataName::Player::posY);

    //プレイヤーを生成
    auto otherplayer = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().Instantiate("Player");
    //描画順を調整して追加
    GetGameObject().lock()->AddChild(otherplayer, true);

    otherplayer->SetPosition({x, y});

    auto player = otherplayer->GetComponent<Player>();
    player->playerNr = playerNr;
    players.push_back(player);
}

//部屋から退場したら消す
void PlayerManager::leaveRoomEventAction(int playerNr, bool isInactive)
{
    auto end = players.end();
    for (auto player = players.begin(); player != end; ++player)
    {
        if ((*player)->playerNr == playerNr)
        {
            GetGameObject().lock()->GetScene().lock()->Destroy((*player)->GetGameObject().lock());

            players.erase(player);

            return;
        }
    }
}

PlayerManager::PlayerManager()
{
    players.reserve(4);
}

void PlayerManager::Start2()
{
    //カメラ位置を設定
    const auto playerpos = players[0]->GetGameObject().lock()->GetPosition();

    auto diff = s3d::Scene::CenterF() - playerpos;

    // altercamera
    GetGameObject().lock()->GetScene().lock()->GetMasterObject()->FindChildByName("AlterCamera")->SetPosition(diff);

    // 送り続ける 1秒に8回
    initsync = Utilities::IntervalCall(1000 / 8, [&]() { players[0]->SendInstantiateMessage(); });
}

void PlayerManager::Update()
{
    if (initsync.IsStop())
    {
        return;
    }

    if (Matching::GameStartTime - networkSystem->GetClient().getServerTime() < 0)
    {
        initsync.Stop();
    }

    initsync.Run();
}
