
#include "Game.hpp"
#include "Enemy/Enemy.hpp"
#include "Enemy/EnemyManager.hpp"
#include "GameMaster/CoreInfoGUI.hpp"
#include "GameMaster/GameState.hpp"
#include "GameMaster/PlayerCore.hpp"
#include "GameMaster/Timer.hpp"
#include "GameMaster/UIManager.hpp"
#include "Player/Player.hpp"
#include "Player/PlayerBulletManager.hpp"
#include "Player/PlayerManager.hpp"
#include "UI/ReturnTitleBt.hpp"

void Game::Setup()
{
    s3d::Scene::SetBackground(s3d::Palette::Whitesmoke);
    //敵の生成番号を初期化
    ResetEnemyNumber();

    //カメラ操作はPlayer.cppのMoveでやってる　要修正〜
    auto Altercamera = CreateAndGetObject();
    Altercamera->SetName("AlterCamera");

    //ゲーム状態管理システムは代替カメラ配下にしない
    auto Gamesystem = CreateAndGetObject();
    Gamesystem->SetName("GameSystem");
    Gamesystem->AddComponent<GameState>();

    //描画順の管理
    auto Bg = Altercamera->CreateChild();
    Bg->SetName("BackGround");
    auto Bulletmanager = Altercamera->CreateChild();
    Bulletmanager->SetName("BulletManager");
    auto Enemymanager = Altercamera->CreateChild();
    Enemymanager->SetName("EnemyManager");
    auto walls = Altercamera->CreateChild();
    walls->SetName("Walls");
    auto Playermanager = Altercamera->CreateChild();
    Playermanager->SetName("PlayerManager");

    //敵管理システム
    auto enemyManagerComp = Enemymanager->AddComponent<EnemyManager>();
    Enemymanager->SetActive(false);

    //自分を作る
    // auto mainplayerobj = GetSceneManager()->GetCommon().Instantiate("Player");
    // mainplayerobj->SetName("MyPlayer");
    // auto system = GetSceneManager()->GetCommon().GetCommonObject(Photon::NetworkObjectName())->GetComponent<Photon::NetworkSystem>();

    // mainplayerobj->SetPosition(Player::playerInitpos[Photon::NetworkSystem::GetInstance()->GetPlayerNumberInRoom()]);

    auto playerManagerComp = Playermanager->AddComponent<PlayerManager>();
    auto player = playerManagerComp->CreateMyPlayer()->GetComponent<Player>();
    // player->SetMine(true);
    // player->playerNr = Photon::NetworkSystem::GetInstance()->GetClient().getLocalPlayer().getNumber();

    // playerManagerComp->players.push_back(player);

    // Playermanager->AddChild(mainplayerobj);

    Bulletmanager->AddComponent<PlayerBulletManager>()->player = player;

    //データ注入
    Enemy::playerManager = playerManagerComp.get();
    Enemy::enemyManager = enemyManagerComp.get();

    // wallscope
    {
        constexpr s3d::Circle bgregion(0, 0, 300);
        //壁を作る
        Bg->AddComponent<Siv::Circle>()->SetColor(s3d::Palette::Whitesmoke.lerp(s3d::Palette::Blue, 0.025)).SetShape(bgregion.scaled(1.3));
        Bg->AddComponent<Siv::Circle>()->SetColor(s3d::Palette::Whitesmoke.lerp(s3d::Palette::Blue, 0.045)).SetShape(bgregion);

        //厚さは600
        //広さは1000
        const auto wallmake = GetSceneManager()->GetCommon().GetInstantiate("MapFieldWall");

        // constexpr int fieldwidth = 400;
        // constexpr int wallwidth = 600 / 2;

        constexpr int offset = 950;
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
    auto core = Altercamera->CreateChild();
    core->SetName("PlayerCore");
    core->SetTag(UserDef::Tag::Wall);
    auto playerCoreComp = core->AddComponent<PlayerCore>();
    core->AddComponent<Collision::CollisionObject>(Collision::Layer::Field);
    constexpr int coresize = 35;
    constexpr s3d::RectF corerect(-coresize, -coresize, coresize * 2, coresize * 2);
    core->AddComponent<Collision::RectCollider>()->SetShape(corerect);
    core->AddComponent<Siv::Rect>()->SetShape(corerect).SetColor(s3d::Palette::Lightgreen);
    core->AddComponent<Siv::RectFrame>()->SetShape(corerect).SetColor(s3d::Palette::Green.lerp(s3d::Palette::Blue, 0.5)).SetInnerThickness(5);
    //コアの位置を通達
    enemyManagerComp->playercore = core;

    // UI
    auto UI = CreateAndGetObject();
    auto uimanager = UI->AddComponent<UIManager>();

    //タイマー
    auto time = UI->CreateChild();
    time->SetPosition({s3d::Scene::Width() - 100, 100});

    time->SetName("Timer");
    time->AddComponent<Siv::Text>()->SetColor(s3d::Palette::Black).SetFont(s3d::Font(50, s3d::Typeface::Bold));
    auto timerComp = time->AddComponent<Timer>();
    timerComp->SetActive(false);
    auto timerSetupComp = time->AddComponent<TimerSetup>();
    timerSetupComp->timerobject = timerComp;
    enemyManagerComp->timer = timerComp;

    auto& statetext = time->CreateChild()->SetName("time text");
    statetext.SetPosition({0, -40});
    statetext.AddComponent<Siv::Text>();

    //ゲーム開始前の説明
    auto explanation = UI->CreateChild();
    explanation->SetPosition(s3d::Scene::CenterF().movedBy(0, -100));
    explanation->AddComponent<Siv::Text>()->SetText(U"中心にあるコアを守れ！").SetFont(s3d::Font(40, s3d::Typeface::Heavy)).SetColor(s3d::Palette::Black);
    timerSetupComp->explanation = explanation;

    {
        //コアのHP表示
        auto Corehpback = UI->CreateChild("CoreHPBack");
        s3d::RectF rect(0, 0, s3d::Scene::Width() * 0.8, 35);
        Corehpback->SetPosition({s3d::Scene::Width() * 0.1, 5});
        Corehpback->AddComponent<Siv::Rect>()->SetShape(rect).SetColor(s3d::ColorF(0.7, 0.7, 0.7, 0.5));
        //説明文字
        Corehpback->CreateChild()
            ->SetPosition({rect.w * 0.03, 1})
            .AddComponent<Siv::Text>()
            ->SetText(U"Defence Object HP")
            .SetFont(s3d::Font(16))
            .SetColor(s3d::Palette::Black)
            .SetDrawAt(false);

        auto Corehp = Corehpback->CreateChild("CoreHP");
        Corehp->SetPosition({rect.w * 0.02, 21});
        rect.w = rect.w * 0.94;
        rect.h = 10;
        auto coregui = Corehp->AddComponent<CoreInfoGUI>();
        (coregui->redbar = Corehp->AddComponent<Siv::Rect>())->SetShape(rect).SetColor(s3d::Palette::Red);
        (coregui->greenbar = Corehp->AddComponent<Siv::Rect>())->SetShape(rect).SetColor(s3d::Palette::Green);
        coregui->core = playerCoreComp;
    }
    //勝利・敗北UI
    auto resultUI = UI->CreateChild("ResultUI");

    auto returnTitleBt = resultUI->CreateChild("TitleBack");
    auto victoryUI = resultUI->CreateChild("victoryUI");
    auto defeatUI = resultUI->CreateChild("defeatUI");
    auto connectionError = resultUI->CreateChild("connectionError");

    returnTitleBt->SetActive(false);
    victoryUI->SetActive(false);
    defeatUI->SetActive(false);
    connectionError->SetActive(false);

    victoryUI->AddComponent<Siv::Text>()->SetText(U"勝利！").SetFont(s3d::Font(100, s3d::Typeface::Black)).SetColor(s3d::Palette::Red).SetDrawAt(true).SetShadowEnable(true);
    victoryUI->SetPosition(s3d::Scene::CenterF());

    defeatUI->AddComponent<Siv::Text>()->SetText(U"敗北…").SetFont(s3d::Font(100, s3d::Typeface::Black)).SetColor(s3d::Palette::Blue).SetDrawAt(true).SetShadowEnable(true);
    defeatUI->SetPosition(s3d::Scene::CenterF());

    connectionError->AddComponent<Siv::Text>()->SetText(U"通信が切断されました").SetFont(s3d::Font(40, s3d::Typeface::Medium)).SetColor(s3d::Palette::Black).SetDrawAt(true);
    connectionError->SetPosition(s3d::Scene::CenterF());

    returnTitleBt->AddComponent<ReturnTitleBt>();
    returnTitleBt->AddComponent<Siv::Button>()->SetText(U"タイトルへ戻る");
    returnTitleBt->SetPosition({s3d::Scene::CenterF().x, s3d::Scene::Height() - 60});

    //依存関係
    timerComp->uimanager = uimanager;
    playerCoreComp->uimanager = uimanager;

    uimanager->objects.playerManager = Playermanager;
    uimanager->objects.enemyManager = Enemymanager;
    uimanager->objects.defeatUI = defeatUI;
    uimanager->objects.victoryUI = victoryUI;
    uimanager->objects.timeUI = time;
    uimanager->objects.returnTitleBt = returnTitleBt;
    uimanager->objects.playerBulletManager = Bulletmanager;
    uimanager->objects.connectError = connectionError;
}
