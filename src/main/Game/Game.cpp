
#include "Game.hpp"

void Game::Setup()
{
    s3d::Scene::SetBackground(s3d::Palette::Lightgrey);

    //描画順を最初にするため弾をここに
    auto bulletmanager = CreateAndGetObject();

    auto players = CreateAndGetObject();
    players->SetName("Players");

    //自分を作る
    auto mainplayerobj = GetSceneManager()->GetCommon().Instantiate("Player");
    auto system = GetSceneManager()->GetCommon().GetCommonObject(Photon::NetworkObjectName())->GetComponent<Photon::NetworkSystem>();

    mainplayerobj->SetPosition(Player::playerInitpos[system->GetPlayerNumberInRoom()]);

    auto player = mainplayerobj->GetComponent<Player>();
    player->SetMine(true);
    player->playerNr = system->GetClient().getLocalPlayer().getNumber();

    players->AddComponent<PlayerMaster>()->players.push_back(player);
    //    player->SendInstantiateMessage();

    players->AddChild(mainplayerobj);

    bulletmanager->AddComponent<BulletManager>()->player = player;
}
