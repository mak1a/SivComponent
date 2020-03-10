
#define NO_USING_S3D

#include "LoadBalancing-cpp/inc/Client.h"
#include <Siv3D.hpp>

namespace PhotonComponent
{
class PhotonListener : public ExitGames::LoadBalancing::Listener
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

    PhotonListener()
        : mState(States::INITIALIZED)
    {
    }
};
}
