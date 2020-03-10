#define NO_S3D_USING
#include "NetworkSystem.hpp"

namespace PhotonComponent
{
    std::string NetworkObjectName()
    {
        return "PhotonSystem";
    }

    NetworkSystem::NetworkSystem()
        : mLoadBalancingClient(*this, appID, appVersion)
    {
        SetState(States::INITIALIZED);

        mLogger.setDebugOutputLevel(ExitGames::Common::DebugLevel::ALL);
        mLogger.setListener(*this);
    }

    NetworkSystem::~NetworkSystem()
    {
        Disconnect();
    }

    void NetworkSystem::Connect(void)
    {
        mLoadBalancingClient.setAutoJoinLobby(true);
        // connect() is asynchronous - the actual result arrives in the Listener::connectReturn() or the Listener::connectionErrorReturn() callback
        if (!mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues().setUserID(ExitGames::Common::JString() + GETTIMEMS()),
                                          PLAYER_NAME + GETTIMEMS()))
            EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
    }

    void NetworkSystem::Update()
    {
        mLoadBalancingClient.service();  // needs to be called regularly!

        static bool playerOutput = true;
        switch (GetState())
        {
            //サーバー接続できたら
            case States::CONNECTED:

                //ルーム接続を行う
                createRoom(L"testroom", 4);
                SetState(States::JOINING);
                break;

            //ルームに入れたら
            case States::JOINED:

                if (playerOutput)
                {
                    OutputPlayers();
                    SendEvent();
                    playerOutput = false;
                }
                break;

            default:
                break;
        }
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
}  // namespace PhotonComponent
