#define NO_S3D_USING
#include "NetworkSystem.hpp"

namespace PhotonComponent
{
    NetworkSystem::NetworkSystem()
        : mLoadBalancingClient(mListener, appID, appVersion)
    {
    }

    NetworkSystem::~NetworkSystem()
    {
        disconnect();
    }

    void NetworkSystem::connect(void)
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
        switch (mListener.GetState())
        {
            //サーバー接続できたら
            case PhotonListener::States::CONNECTED:

                //ルーム接続を行う
                createRoom(L"testroom", 4);
                mListener.SetState(PhotonListener::States::JOINING);
                break;

            //ルームに入れたら
            case PhotonListener::States::JOINED:

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

    void NetworkSystem::disconnect(void)
    {
        mLoadBalancingClient.disconnect();  // disconnect() is asynchronous - the actual result arrives in the Listener::disconnectReturn() callback
    }

    //シーン変更するだけのクラス
    class SceneChanger : public ComponentEngine::AttachableComponent
    {
        void Update() override
        {
            gameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
        }
    };
}  // namespace PhotonComponent
