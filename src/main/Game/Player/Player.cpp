
#include "Player.hpp"
#include "../../../CustomEventList.hpp"
#include "../../../Matching.hpp"
// #include "../../../SivComponent/SivComponent.hpp"

using dictype = ExitGames::Common::Dictionary<nByte, double>;

Player::Player()
    //秒間10同期
    : syncpos((int32_t)(1000 / 15), [&]() { SyncPos(); })
{
    isMine = false;
    spd = 50.0;
}

void Player::Start2()
{
    targetPos = GetGameObject().lock()->GetPosition();

    constexpr auto playercolor = s3d::Palette::Limegreen.lerp(s3d::Palette::Lightblue, 0.5);
    constexpr auto othercolor = s3d::Palette::Limegreen.lerp(s3d::Palette::Black, 0.2);

    GetGameObject().lock()->GetComponent<Siv::Rect>()->SetColor(isMine ? playercolor : othercolor);

    master = GetGameObject().lock()->GetScene().lock()->GetMasterObject();
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
    auto pos = obj->GetPosition();
    pos += axis * spd * s3d::Scene::DeltaTime();
    obj->SetPosition(pos);

    auto x = master->GetPosition() - axis * spd * s3d::Scene::DeltaTime();
    master->SetPosition(x);
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

s3d::Vec2 Player::playerInitpos[4] = {s3d::Vec2(200, 200), s3d::Vec2(400, 200), s3d::Vec2(200, 400), s3d::Vec2(400, 400)};

//----------------------------------

//プレイヤーを生成
void PlayerMaster::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
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
void PlayerMaster::leaveRoomEventAction(int playerNr, bool isInactive)
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

void PlayerMaster::Start2()
{
    players.reserve(4);

    //送り続ける
    initsync = Utilities::IntervalCall(50, [&]() { players[0]->SendInstantiateMessage(); });
}

void PlayerMaster::Update()
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
