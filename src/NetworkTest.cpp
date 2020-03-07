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
        s3d::Print(U"debugReturn");
        s3d::Print(string.toString());
    };

    void connectionErrorReturn(int errorCode) override
    {
        s3d::Print(U"connectionErrorReturn");
    };

    void clientErrorReturn(int errorCode) override
    {
        s3d::Print(U"clientErrorReturn");
    };

    void warningReturn(int warningCode) override
    {
        s3d::Print(U"warningReturn");
    };

    void serverErrorReturn(int errorCode) override
    {
        s3d::Print(U"serverErrorReturn");
    };

    void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override
    {
        s3d::Print(U"joinRoomEventAction");
        mState = States::JOINED;
    };

    void leaveRoomEventAction(int playerNr, bool isInactive) override
    {
        s3d::Print(U"leaveRoomEventAction");
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

            mState = States::DISCONNECTING;
            return;
        }
        s3d::Print(U"connected!");
        mState = States::CONNECTED;
    };

    void disconnectReturn() override
    {
        s3d::Print(U"disconnectReturn");
    };

    void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override
    {
        s3d::Print(U"leaveRoomReturn");
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
        mLoadBalancingClient.opJoinOrCreateRoom(roomName, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers));
    }

    void OutputPlayers()
    {
        auto players = mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers();
        auto l = players.getSize();
        for (int k = 0; k < l; ++k)
        {
            s3d::Print(players[k]->getName());
        }
    }

    void SendEvent()
    {
        auto eventhash = new ExitGames::Common::Hashtable();
        auto senddata = L"I am " + mLoadBalancingClient.getLocalPlayer().getName();
        mLoadBalancingClient.opRaiseEvent(true, senddata, 10);
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

        //ルームに入れたら
        case SampleListener::States::JOINED:

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
