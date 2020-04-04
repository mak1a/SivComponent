
#include "Matching.hpp"

#include "MatchingButtons.hpp"

using namespace ComponentEngine;

int Matching::GameStartTime = 0;
int Matching::Difficulty = 3;

void Matching::Setup()
{
    auto sys = CreateAndGetObject()->AddComponent<MatchSystem>();
    auto list = CreateAndGetObject();
    list->SetPosition({200, 100});
    list->AddComponent<PlayerListDisplay>();
    list->AddComponent<Siv::Text>()->SetText(U"しばらくお待ちください").SetDrawAt(false);

    auto bt = CreateAndGetObject();
    bt->AddComponent<TitleBt>();
    bt->SetPosition({200, s3d::Scene::Height() - 60});

    bt = CreateAndGetObject();
    bt->AddComponent<GameBt>()->match = this;
    bt->AddComponent<ShowButtonOnlyMaster>();
    bt->SetPosition({s3d::Scene::Width() - 200, s3d::Scene::Height() - 60});

    //難易度調整機構
    auto ui = CreateAndGetObject();
    ui->SetPosition({s3d::Scene::CenterF().x, s3d::Scene::Height() - 180});

    auto display = ui->CreateChild();
    display->AddComponent<Siv::Text>()->SetFont(s3d::Font(30)).SetColor(s3d::Palette::Black);
    display->AddComponent<DifficultyDisplay>();

    auto plus = ui->CreateChild();
    plus->SetPosition({200, 0});
    plus->AddComponent<Siv::Button>()
        ->SetText(U"+")
        .SetWidth(50)
        .SetDelegate([]() {
            Matching::Difficulty = std::min(10, Matching::Difficulty + 1);

            Matching::SyncRoomInfo();
        })
        .SetActive(false);
    plus->AddComponent<ShowButtonOnlyMaster>();

    auto minus = ui->CreateChild();
    minus->SetPosition({-200, 0});
    minus->AddComponent<Siv::Button>()
        ->SetText(U"-")
        .SetWidth(50)
        .SetDelegate([]() {
            Matching::Difficulty = std::max(1, Matching::Difficulty - 1);

            Matching::SyncRoomInfo();
        })
        .SetActive(false);
    minus->AddComponent<ShowButtonOnlyMaster>();

    // photonの操作をできるように
    system = GetSceneManager()->GetCommon().GetCommonObject(Photon::NetworkObjectName())->GetComponent<Photon::NetworkSystem>();
}

void Matching::SyncRoomInfo(bool gameStart)
{
    ExitGames::Common::Hashtable table;
    table.put<short, int>(static_cast<short>(0), Difficulty);

    table.put<short, bool>(static_cast<short>(90), gameStart);

    const int starttime = Photon::NetworkSystem::GetInstance()->GetClient().getServerTime() + 5000;
    table.put<short, int>(static_cast<short>(1), starttime);
    Matching::GameStartTime = starttime;

    Photon::NetworkSystem::GetInstance()->GetClient().opRaiseEvent(true, table, CustomEvent::MatchingSync);
}