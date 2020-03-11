

#pragma once

#include "PhotonComponent/PhotonComponent.hpp"

class MatchSystem : public PhotonComponent::AttachableComponentPhotonCallbacks
{
    std::shared_ptr<PhotonComponent::NetworkSystem> system;

    void Start() override
    {
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommonObject("PhotonSystem");
        system = object->GetComponent<PhotonComponent::NetworkSystem>();
        system->Connect();
    }

    void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override
    {
        s3d::Print(U"joinRoomEventAction");
        system->OutputPlayers();
        system->SendEvent();
    };

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

    void disconnectReturn() override
    {
        s3d::Print(U"disconnectReturn");
    };

    void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override
    {
        s3d::Print(U"leaveRoomReturn");
    };
};

class Matching : public ComponentEngine::IScene
{
    void Setup()
    {
        CreateAndGetObject()->AddComponent<MatchSystem>();
    }
};
