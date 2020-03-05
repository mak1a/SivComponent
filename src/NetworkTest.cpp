#include <iostream>

#define NO_S3D_USING
#include <Siv3D.hpp>
#include "LoadBalancing-cpp/inc/Client.h"
#include "NetworkTest.hpp"

static const ExitGames::Common::JString PLAYER_NAME = L"user";

class SampleListener : public ExitGames::LoadBalancing::Listener
{
    void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override
    {
        std::cout << "debugReturn" << std::endl;
    };

    void connectionErrorReturn(int errorCode) override
    {
        std::cout << "connectionErrorReturn" << std::endl;
    };

    void clientErrorReturn(int errorCode) override
    {
        std::cout << "clientErrorReturn" << std::endl;
    };

    void warningReturn(int warningCode) override
    {
        std::cout << "warningReturn" << std::endl;
    };

    void serverErrorReturn(int errorCode) override
    {
        std::cout << "serverErrorReturn" << std::endl;
    };

    void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override
    {
        std::cout << "joinRoomEventAction" << std::endl;
        mState = States::JOINED;
    };

    void leaveRoomEventAction(int playerNr, bool isInactive) override
    {
        std::cout << "leaveRoomEventAction" << std::endl;
    };

    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override
    {
        std::cout << "customEventAction" << std::endl;
    };

    void connectReturn(int errorCode,
                       const ExitGames::Common::JString& errorString,
                       const ExitGames::Common::JString& region,
                       const ExitGames::Common::JString& cluster) override
    {
        if (errorCode)
        {
            std::cout << "connect error" << std::endl;

            mState = States::DISCONNECTING;
            return;
        }
        std::cout << "connected!" << std::endl;
        mState = States::CONNECTED;
    };

    void disconnectReturn() override
    {
        std::cout << "disconnectReturn" << std::endl;
    };

    void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override
    {
        std::cout << "leaveRoomReturn" << std::endl;
    };

public:
    enum class States
    {
        INITIALIZED = 0,
        CONNECTING,
        CONNECTED,
        JOINING,
        JOINED,
        SENT_DATA,
        RECEIVED_DATA,
        LEAVING,
        LEFT,
        DISCONNECTING,
        DISCONNECTED
    } mState;

public:
    States GetState()
    {
        return mState;
    }

    void SetState(States state)
    {
        mState = state;
    }

    SampleListener()
        : mState(States::INITIALIZED)
    {
    }
};

class SampleNetworkLogic
{
public:
    SampleNetworkLogic(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion);
    void connect(void);
    void disconnect(void);
    void run(void);

    void createRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers)
    {
        mLoadBalancingClient.opCreateRoom(roomName, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers));
    }

    void OutputPlayers()
    {
        auto players = mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers();
        auto l = players.getSize();
        for (int k = 0; k < l; ++k)
        {
            std::wcout << players[k]->getName() << std::endl;
        }
    }

private:
    ExitGames::LoadBalancing::Client mLoadBalancingClient;
    SampleListener mListener;           // your implementation of the ExitGames::LoadBalancing::Listener interface
    ExitGames::Common::Logger mLogger;  // accessed by EGLOG()
};

SampleNetworkLogic::SampleNetworkLogic(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion)
    : mLoadBalancingClient(mListener, appID, appVersion)
{
}

void SampleNetworkLogic::connect(void)
{
    mLoadBalancingClient.setAutoJoinLobby(true);
    // connect() is asynchronous - the actual result arrives in the Listener::connectReturn() or the Listener::connectionErrorReturn() callback
    if (!mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues().setUserID(ExitGames::Common::JString() + GETTIMEMS()),
                                      PLAYER_NAME + GETTIMEMS()))
        EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
}

void SampleNetworkLogic::run(void)
{
    mLoadBalancingClient.service();  // needs to be called regularly!
    static bool playerOutput = true;
    switch (mListener.GetState())
    {
        //サーバー接続できたら
        case SampleListener::States::CONNECTED:

            //ルーム接続を行う
            createRoom(L"testroom", 4);
            mListener.SetState(SampleListener::States::JOINING);
            break;

        case SampleListener::States::JOINED:

            if (playerOutput)
            {
                OutputPlayers();
                playerOutput = false;
            }
            break;

        default:
            break;
    }
}

void SampleNetworkLogic::disconnect(void)
{
    mLoadBalancingClient.disconnect();  // disconnect() is asynchronous - the actual result arrives in the Listener::disconnectReturn() callback
}

void PhotonTest()
{
    static const ExitGames::Common::JString appID = L"b0f7b045-6e8c-4dd7-a671-e5aaeae5c2f4";  // set your app id here
    static const ExitGames::Common::JString appVersion = L"1.0";

    SampleNetworkLogic networkLogic(appID, appVersion);

    networkLogic.connect();

    while (s3d::System::Update())
    {
        networkLogic.run();
        if (s3d::KeySpace.down())
        {
            break;
        }
    }
    networkLogic.disconnect();

    while (s3d::System::Update())
    {
        networkLogic.run();
        if (s3d::KeySpace.down())
        {
            break;
        }
    }
}
