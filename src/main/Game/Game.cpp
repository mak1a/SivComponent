
#include "Game.hpp"

void Game::Setup()
{
    s3d::Scene::SetBackground(s3d::Palette::Whitesmoke);

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

    //壁を作る

    //厚さは600
    //広さは1000
    const auto wallmake = GetSceneManager()->GetCommon().GetInstantiate("MapFieldWall");

    auto walls = CreateAndGetObject();
    // scope
    {
        constexpr int fieldwidth = 400;
        constexpr int wallwidth = 600 / 2;

        auto top = wallmake();
        top->SetPosition({300 - fieldwidth - wallwidth, 300});

        auto bottom = wallmake();
        bottom->SetPosition({300 + fieldwidth + wallwidth, 300});

        auto right = wallmake();
        right->SetPosition({300, 300 + fieldwidth + wallwidth}).SetRotateByAngle(90);

        auto left = wallmake();
        left->SetPosition({300, 300 - fieldwidth - wallwidth}).SetRotateByAngle(90);

        walls->AddChild(top);
        walls->AddChild(bottom);
        walls->AddChild(left);
        walls->AddChild(right);
    }
}
