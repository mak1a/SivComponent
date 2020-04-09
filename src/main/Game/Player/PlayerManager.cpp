
#include "PlayerManager.hpp"
#include "../../Common/CommonMemory.hpp"
#include "../../CustomEventList.hpp"
#include "../../Matching/Matching.hpp"
#include "Player.hpp"

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

    auto dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int> >(eventContent).getDataCopy();

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

std::shared_ptr<GameObject> PlayerManager::CreateMyPlayer()
{
    //自分を作る
    auto mainplayerobj = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().Instantiate("Player");
    mainplayerobj->SetName("MyPlayer");

    mainplayerobj->SetPosition(Player::playerInitpos[Photon::NetworkSystem::GetInstance()->GetPlayerNumberInRoom()]);

    auto player = mainplayerobj->GetComponent<Player>();
    player->SetMine(true);
    player->playerNr = Photon::NetworkSystem::GetInstance()->GetClient().getLocalPlayer().getNumber();

    this->players.push_back(player);

    GetGameObject().lock()->AddChild(mainplayerobj);

    return mainplayerobj;
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

    if (CommonMemory::GetGameStartTime() - networkSystem->GetClient().getServerTime() < 0)
    {
        initsync.Stop();
    }

    initsync.Run();
}
