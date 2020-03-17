
#include "Matching.hpp"

#define NO_S3D_USING
#include "CustomEventList.hpp"
#include "PhotonComponent/PhotonComponent.hpp"
#include "SivComponent/SivComponent.hpp"
#include "Utilities/PhotonUtilities.hpp"

using namespace ComponentEngine;

class PlayerListDisplay : public ComponentEngine::Photon::AttachableComponentPhotonCallbacks
{
    std::shared_ptr<ComponentEngine::Photon::NetworkSystem> system;

    std::shared_ptr<Siv::Text> text;

    void Start() override
    {
        text = GetGameObject().lock()->GetComponent<Siv::Text>();

        system = GetGameObject()
                     .lock()
                     ->GetScene()
                     .lock()
                     ->GetSceneManager()
                     ->GetCommon()
                     .GetCommonObject("PhotonSystem")
                     ->GetComponent<ComponentEngine::Photon::NetworkSystem>();
    }

    void UpdateDisplay()
    {
        auto plist = system->GetPlayerList();

        s3d::String str;
        for (ExitGames::LoadBalancing::Player* player : plist)
        {
            int num = Utilities::GetPlayerNumber(player->getName());
            str += s3d::Format(player->getNumber(), U" : Player", num, U"\n");
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

    void Start() override
    {
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetCommonObject("PhotonSystem");
        system = object->GetComponent<ComponentEngine::Photon::NetworkSystem>();
        system->Connect();
    }

    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override
    {
        s3d::Print(U"customEventAction");

        if (eventCode == 10)
        {
            s3d::Print(ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).toString());
        }
    };

    void connectReturn(int errorCode,
                       const ExitGames::Common::JString& errorString,
                       const ExitGames::Common::JString& region,
                       const ExitGames::Common::JString& cluster) override
    {
        if (errorCode)
        {
            s3d::Print(U"connect error");

            GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
            return;
        }
        system->createRoom(L"testroom", 4);
        s3d::Print(U"connected!");
    };

    void joinOrCreateRoomReturn(int localPlayerNr,
                                const ExitGames::Common::Hashtable& roomProperties,
                                const ExitGames::Common::Hashtable& playerProperties,
                                int errorCode,
                                const ExitGames::Common::JString& errorString) override
    {
        if (errorCode)
        {
            system->Disconnect();
            GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
        }
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

    void Start() override
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

        system->GetClient().opRaiseEvent(true, 0, CustomEvent::GameStart);
        system->GetClient().getCurrentlyJoinedRoom().setIsOpen(false);
        manager->ChangeScene("Game");
    }

    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override
    {
        if (eventCode == CustomEvent::GameStart)
        {
            GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Game");
        }
    }
};

void Matching::Setup()
{
    auto sys = CreateAndGetObject()->AddComponent<MatchSystem>();
    auto list = CreateAndGetObject();
    list->transform().SetPosition({100, 100});
    list->AddComponent<PlayerListDisplay>();
    list->AddComponent<Siv::Text>()->SetText(U"マッチング中…");

    auto bt = CreateAndGetObject();
    bt->AddComponent<TitleBt>();
    bt->transform().SetPosition({200, s3d::Scene::Height() - 60});

    bt = CreateAndGetObject();
    bt->AddComponent<GameBt>();
    bt->transform().SetPosition({s3d::Scene::Width() - 200, s3d::Scene::Height() - 60});

    // for (int i = 0; i < 4; ++i)
    // {
    //     auto col = CreateAndGetObject();
    //     list->AddComponent<PlayerListDisplay>();
    //     list->AddComponent<Siv::Text>()->SetText(U"マッチング中…");
    // }
}