
#include "Matching.hpp"

#define NO_S3D_USING
#include "CustomEventList.hpp"
#include "PhotonComponent/PhotonComponent.hpp"
#include "SivComponent/SivComponent.hpp"
#include "Utilities/PhotonUtilities.hpp"

using namespace ComponentEngine;

int Matching::GameStartTime = 0;

class PlayerListDisplay : public ComponentEngine::Photon::AttachableComponentPhotonCallbacks
{
    std::shared_ptr<ComponentEngine::Photon::NetworkSystem> system;

    std::shared_ptr<Siv::Text> text;

    void Start2() override
    {
        text = GetGameObject().lock()->GetComponent<Siv::Text>();
    }

    void UpdateDisplay()
    {
        auto plist = networkSystem->GetPlayerList();

        s3d::String str;
        for (int i = 0; i < plist.size(); ++i)
        {
            int num = Utilities::GetPlayerNumber(plist[i]->getName());
            str += s3d::Format(i + 1, U" : Player", num, U"\n");
        }
        text->SetText(str);
    }

    void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override
    {
        UpdateDisplay();
    };

    void leaveRoomEventAction(int playerNr, bool isInactive) override
    {
        UpdateDisplay();
    }
};

class MatchSystem : public ComponentEngine::Photon::AttachableComponentPhotonCallbacks
{
    std::shared_ptr<ComponentEngine::Photon::NetworkSystem> system;

    void Start2() override
    {
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetCommonObject("PhotonSystem");

        networkSystem->Connect();
        s3d::Print(U"connecting...");

        networkSystem->GetClient().fetchServerTimestamp();
    }

    // void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override
    // {
    //     s3d::Print(U"customEventAction");

    //     if (eventCode == 10)
    //     {
    //         s3d::Print(ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).toString());
    //     }
    // };

    void connectReturn(int errorCode,
                       const ExitGames::Common::JString& errorString,
                       const ExitGames::Common::JString& region,
                       const ExitGames::Common::JString& cluster) override
    {
        if (errorCode)
        {
            s3d::Print(U"connect error.");

            GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
            return;
        }
        s3d::Print(U"connected!");

        // networkSystem->GetClient().opJoinRandomOrCreateRoom(L"", ExitGames::LoadBalancing::RoomOptions(), ExitGames::Common::Hashtable(), 4);

        networkSystem->GetClient().opJoinRandomRoom(ExitGames::Common::Hashtable(), 4);

        s3d::Print(U"Search Room...");
    };

    void joinRandomRoomReturn(int localPlayerNr,
                              const ExitGames::Common::Hashtable& roomProperties,
                              const ExitGames::Common::Hashtable& playerProperties,
                              int errorCode,
                              const ExitGames::Common::JString& errorString) override

    {
        if (errorCode)
        {
            s3d::Print(U"Room not found.");

            networkSystem->GetClient().opCreateRoom(L"", ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(4));

            s3d::Print(U"Create Room...");
        }
        s3d::Print(U"Connected Room!");
    }

    void joinRoomReturn(int localPlayerNr,
                        const ExitGames::Common::Hashtable& roomProperties,
                        const ExitGames::Common::Hashtable& playerProperties,
                        int errorCode,
                        const ExitGames::Common::JString& errorString) override
    {
        if (errorCode)
        {
            s3d::Print(U"CreateRoom error!");
            s3d::Print(U"Please back to Title.");

            GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
            return;
        }

        s3d::Print(U"Join Room!");
    }

    // void disconnectReturn() override
    // {
    //     s3d::Print(U"disconnectReturn");
    // }

    // void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override
    // {
    //     s3d::Print(U"leaveRoomReturn");
    // }

    void OnDestory2() override
    {
        s3d::ClearPrint();
    }
};

class TitleBt : public ComponentEngine::AttachableComponent
{
    void Start()
    {
        auto bt = GetGameObject().lock()->AddComponent<ComponentEngine::Siv::Button>();
        bt->SetText(U"タイトルへ戻る");
        bt->SetDelegate([=]() { this->ReturnTitle(); });
    }

public:
    void ReturnTitle()
    {
        auto manager = GetGameObject().lock()->GetScene().lock()->GetSceneManager();
        auto system = manager->GetCommon().GetCommonObject("PhotonSystem")->GetComponent<ComponentEngine::Photon::NetworkSystem>();
        system->Disconnect();
        manager->ChangeScene("Title");
    }
};
class GameBt : public Photon::AttachableComponentPhotonCallbacks
{
    std::shared_ptr<Photon::NetworkSystem> system;
    std::shared_ptr<Siv::Button> bt;

    void Start2() override
    {
        system = GetGameObject()
                     .lock()
                     ->GetScene()
                     .lock()
                     ->GetSceneManager()
                     ->GetCommon()
                     .GetCommonObject("PhotonSystem")
                     ->GetComponent<ComponentEngine::Photon::NetworkSystem>();

        //ボタンコンポーネント追加
        bt = GetGameObject().lock()->AddComponent<Siv::Button>();
        bt->SetText(U"ゲームスタート");
        bt->SetDelegate([=]() { this->StartGame(); });
        bt->SetActive(system->GetIsMasterClient());
    }

    void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override
    {
        bt->SetActive(system->GetIsMasterClient());
    };

    void leaveRoomEventAction(int playerNr, bool isInactive) override
    {
        bt->SetActive(system->GetIsMasterClient());
    }

public:
    void StartGame()
    {
        auto manager = GetGameObject().lock()->GetScene().lock()->GetSceneManager();

        const int starttime = networkSystem->GetClient().getServerTime() + 5000;

        system->GetClient().opRaiseEvent(true, starttime, CustomEvent::GameStart);

        Matching::GameStartTime = starttime;

        system->GetClient().getCurrentlyJoinedRoom().setIsOpen(false);
        manager->ChangeScene("Game");
    }

    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override
    {
        if (eventCode != CustomEvent::GameStart)
        {
            return;
        }
        Matching::GameStartTime = ExitGames::Common::ValueObject<int>(eventContent).getDataCopy();
        GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Game");
    }
};

void Matching::Setup()
{
    auto sys = CreateAndGetObject()->AddComponent<MatchSystem>();
    auto list = CreateAndGetObject();
    list->SetPosition({200, 100});
    list->AddComponent<PlayerListDisplay>();
    list->AddComponent<Siv::Text>()->SetText(U"しばらくお待ちください");

    auto bt = CreateAndGetObject();
    bt->AddComponent<TitleBt>();
    bt->SetPosition({200, s3d::Scene::Height() - 60});

    bt = CreateAndGetObject();
    bt->AddComponent<GameBt>();
    bt->SetPosition({s3d::Scene::Width() - 200, s3d::Scene::Height() - 60});

    // for (int i = 0; i < 4; ++i)
    // {
    //     auto col = CreateAndGetObject();
    //     list->AddComponent<PlayerListDisplay>();
    //     list->AddComponent<Siv::Text>()->SetText(U"マッチング中…");
    // }
}
