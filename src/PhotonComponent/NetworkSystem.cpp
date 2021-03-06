﻿#define NO_S3D_USING
#include "NetworkSystem.hpp"

namespace ComponentEngine::Photon
{
    NetworkSystem* NetworkSystem::instance = nullptr;

    std::string NetworkObjectName()
    {
        return "PhotonSystem";
    }

    [[nodiscard]] s3d::String ConvertJStringToString(const ExitGames::Common::JString& str) {
        return s3d::Unicode::FromWString(std::wstring(str));
    }

    [[nodiscard]] ExitGames::Common::JString ConvertStringToJString(const s3d::String& str) {
        return ExitGames::Common::JString(str.toWstr().c_str());
    }

    NetworkSystem::NetworkSystem()
        : mLoadBalancingClient(*this, ChangeAppIDString(), appVersion, ExitGames::Photon::ConnectionProtocol::UDP)
    {
        SetPlayerName(L"null player");

        mLogger.setDebugOutputLevel(ExitGames::Common::DebugLevel::ALL);
        mLogger.setListener(*this);

        //しっかりとしたシングルトンにしたい
        instance = this;
    }

    NetworkSystem::~NetworkSystem()
    {
        Disconnect();

        instance = nullptr;
    }

    void NetworkSystem::Connect(void)
    {
        mLoadBalancingClient.setAutoJoinLobby(true);
        // connect() is asynchronous - the actual result arrives in the Listener::connectReturn() or the Listener::connectionErrorReturn() callback
        if (!mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues().setUserID(ExitGames::Common::JString() + GETTIMEMS()), playerName))
            EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
    }

    void NetworkSystem::Update()
    {
        mLoadBalancingClient.service();  // needs to be called regularly!
    }

    void NetworkSystem::OnDestroy()
    {
        Disconnect();
    }

    void NetworkSystem::Disconnect(void)
    {
        mLoadBalancingClient.disconnect();  // disconnect() is asynchronous - the actual result arrives in the Listener::disconnectReturn() callback
    }

    //シーン変更するだけのクラス
    class SceneChanger : public ComponentEngine::AttachableComponent
    {
        void Update() override
        {
            GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
        }
    };
}  // namespace ComponentEngine::Photon
