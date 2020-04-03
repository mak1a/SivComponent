#pragma once

#include "../../ComponentEngine/ComponentEngine.hpp"
#include "../../PhotonComponent/PhotonComponent.hpp"

#include "../../Utilities/PhotonUtilities.hpp"
#include "../CustomEventList.hpp"
#include "Matching.hpp"

class PlayerListDisplay : public ComponentEngine::Photon::AttachableComponentPhotonCallbacks
{
    std::shared_ptr<ComponentEngine::Photon::NetworkSystem> system;

    std::shared_ptr<Siv::Text> text;

    void Start2() override;

    void UpdateDisplay();

    void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override;

    void leaveRoomEventAction(int playerNr, bool isInactive) override;
};

class MatchSystem : public ComponentEngine::Photon::AttachableComponentPhotonCallbacks
{
    std::shared_ptr<ComponentEngine::Photon::NetworkSystem> system;

    void Start2() override;

    void connectReturn(int errorCode,
                       const ExitGames::Common::JString& errorString,
                       const ExitGames::Common::JString& region,
                       const ExitGames::Common::JString& cluster) override;

    void joinRandomRoomReturn(int localPlayerNr,
                              const ExitGames::Common::Hashtable& roomProperties,
                              const ExitGames::Common::Hashtable& playerProperties,
                              int errorCode,
                              const ExitGames::Common::JString& errorString) override;

    void joinRoomReturn(int localPlayerNr,
                        const ExitGames::Common::Hashtable& roomProperties,
                        const ExitGames::Common::Hashtable& playerProperties,
                        int errorCode,
                        const ExitGames::Common::JString& errorString) override;

    // void disconnectReturn() override
    // {
    //     s3d::Print(U"disconnectReturn");
    // }

    // void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override
    // {
    //     s3d::Print(U"leaveRoomReturn");
    // }

    void OnDestory2() override;

    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;
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

class ShowButtonOnlyMaster : public Photon::AttachableComponentPhotonCallbacks
{
    void Start2() override
    {
        bt = GetGameObject().lock()->GetComponent<Siv::Button>();
    }

    void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override
    {
        bt->SetActive(networkSystem->GetIsMasterClient());
    };

    void leaveRoomEventAction(int playerNr, bool isInactive) override
    {
        bt->SetActive(networkSystem->GetIsMasterClient());
    }

public:
    std::shared_ptr<Siv::Button> bt;
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
        // bt->SetActive(system->GetIsMasterClient());
        match->SyncRoomInfo();
    };

    void leaveRoomEventAction(int playerNr, bool isInactive) override
    {
        // bt->SetActive(system->GetIsMasterClient());
    }

public:
    void StartGame()
    {
        auto manager = GetGameObject().lock()->GetScene().lock()->GetSceneManager();

        match->SyncRoomInfo(true);

        system->GetClient().getCurrentlyJoinedRoom().setIsOpen(false);
        manager->ChangeScene("Game");
    }

    Matching* match;
};

class DifficultyDisplay : public AttachableComponent
{
    std::shared_ptr<Siv::Text> text;

    void Start()
    {
        text = GetGameObject().lock()->GetComponent<Siv::Text>();
    }

    void Update()
    {
        text->SetText(s3d::ToString(Matching::Difficulty));
    }
};
