
#include "Game.hpp"
#include "Enemy/EnemyManager.hpp"
#include "GameMaster/GameState.hpp"
#include "GameMaster/PlayerCore.hpp"
#include "GameMaster/Timer.hpp"
#include "Player/PlayerBulletManager.hpp"

void Game::Setup()
{
    // system

    s3d::Scene::SetBackground(s3d::Palette::Whitesmoke);

    auto altercamera = CreateAndGetObject();
    altercamera->SetName("AlterCamera");

    //ゲーム状態管理システムは代替カメラ配下にしない
    auto gameSystem = CreateAndGetObject();
    gameSystem->SetName("GameSystem");
    gameSystem->AddComponent<GameState>();

    //描画順を最初にするため弾をここに
    auto bulletmanager = altercamera->CreateChild();
    bulletmanager->SetName("BulletManager");

    //敵管理システム
    auto enemyManager = altercamera->CreateChild();
    enemyManager->SetName("EnemyManager");
    enemyManager->AddComponent<EnemyManager>();
    enemyManager->SetActive(false);

    auto players = altercamera->CreateChild();
    players->SetName("PlayerManager");

    //自分を作る
    auto mainplayerobj = GetSceneManager()->GetCommon().Instantiate("Player");
    mainplayerobj->SetName("MyPlayer");
    auto system = GetSceneManager()->GetCommon().GetCommonObject(Photon::NetworkObjectName())->GetComponent<Photon::NetworkSystem>();

    mainplayerobj->SetPosition(Player::playerInitpos[system->GetPlayerNumberInRoom()]);

    auto player = mainplayerobj->GetComponent<Player>();
    player->SetMine(true);
    player->playerNr = system->GetClient().getLocalPlayer().getNumber();

    players->AddComponent<PlayerManager>()->players.push_back(player);
    //    player->SendInstantiateMessage();

    players->AddChild(mainplayerobj);

    bulletmanager->AddComponent<PlayerBulletManager>()->player = player;

    auto walls = altercamera->CreateChild();
    walls->SetName("Walls");
    // wallscope
    {
        //壁を作る

        //厚さは600
        //広さは1000
        const auto wallmake = GetSceneManager()->GetCommon().GetInstantiate("MapFieldWall");

        // constexpr int fieldwidth = 400;
        // constexpr int wallwidth = 600 / 2;

        constexpr int offset = 900;
        auto top = wallmake();
        top->SetPosition({0, -offset});

        auto bottom = wallmake();
        bottom->SetPosition({0, offset});

        auto right = wallmake();
        right->SetPosition({offset, 0}).SetRotateByAngle(90);

        auto left = wallmake();
        left->SetPosition({-offset, 0}).SetRotateByAngle(90);

        walls->AddChild(top);
        walls->AddChild(bottom);
        walls->AddChild(left);
        walls->AddChild(right);
    }

    //プレイヤーコアを生成
    auto core = altercamera->CreateChild();
    core->SetTag(UserDef::Tag::Wall);
    core->AddComponent<PlayerCore>();
    core->AddComponent<Collision::CollisionObject>(Collision::Layer::Field);
    constexpr int coresize = 50;
    constexpr s3d::RectF corerect(-coresize, -coresize, coresize * 2, coresize * 2);
    core->AddComponent<Collision::RectCollider>()->SetShape(corerect);
    core->AddComponent<Siv::Rect>()->SetShape(corerect).SetColor(s3d::Palette::Green);

    // UI

    //タイマー
    auto time = CreateAndGetObject();
    time->SetPosition({s3d::Scene::Width() / 2, 70});

    time->SetName("Timer");
    time->AddComponent<Siv::Text>()->SetColor(s3d::Palette::Black).SetFont(s3d::Font(50, s3d::Typeface::Bold));
    auto t = time->AddComponent<Timer>();
    t->SetActive(false);
    time->AddComponent<TimerSetup>()->timerobject = t;

    auto& statetext = time->CreateChild()->SetName("time text");
    statetext.SetPosition({0, -40});
    statetext.AddComponent<Siv::Text>();
}
