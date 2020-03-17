#pragma once

//#include"ComponentEngine.hpp"
#include "../ComponentEngine/AttachableComponent.hpp"
#include "../ComponentEngine/IScene.cpp"

#include <LoadBalancing-cpp/inc/Client.h>

namespace ComponentEngine::Photon
{
    class NetworkSystem;

    class AttachableComponentPhotonCallbacks : public ComponentEngine::AttachableComponent
    {
    protected:
        NetworkSystem* networkSystem;
        friend NetworkSystem;

    public:
        void Start() override final;
        virtual void Start2(){};
        virtual void OnDestroy() override final;
        virtual void OnDestory2(){};
        // ~AttachableComponentPhotonCallbacks();

        // receive and print out debug out here
        virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string) {}

        // implement your error-handling here
        virtual void connectionErrorReturn(int errorCode) {}
        virtual void clientErrorReturn(int errorCode) {}
        virtual void warningReturn(int warningCode) {}
        virtual void serverErrorReturn(int errorCode) {}

        // events, triggered by certain operations of all players in the same room
        virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) {}
        virtual void leaveRoomEventAction(int playerNr, bool isInactive) {}

        virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) {}

        // callbacks for operations on the server
        virtual void connectReturn(int errorCode,
                                   const ExitGames::Common::JString& errorString,
                                   const ExitGames::Common::JString& region,
                                   const ExitGames::Common::JString& cluster)
        {
        }
        virtual void disconnectReturn(void) {}
        virtual void createRoomReturn(int localPlayerNr,
                                      const ExitGames::Common::Hashtable& roomProperties,
                                      const ExitGames::Common::Hashtable& playerProperties,
                                      int errorCode,
                                      const ExitGames::Common::JString& errorString)
        {
        }
        virtual void joinOrCreateRoomReturn(int localPlayerNr,
                                            const ExitGames::Common::Hashtable& roomProperties,
                                            const ExitGames::Common::Hashtable& playerProperties,
                                            int errorCode,
                                            const ExitGames::Common::JString& errorString)
        {
        }
        virtual void joinRandomOrCreateRoomReturn(int localPlayerNr,
                                                  const ExitGames::Common::Hashtable& roomProperties,
                                                  const ExitGames::Common::Hashtable& playerProperties,
                                                  int errorCode,
                                                  const ExitGames::Common::JString& errorString)
        {
        }
        virtual void joinRoomReturn(int localPlayerNr,
                                    const ExitGames::Common::Hashtable& roomProperties,
                                    const ExitGames::Common::Hashtable& playerProperties,
                                    int errorCode,
                                    const ExitGames::Common::JString& errorString)
        {
        }
        virtual void joinRandomRoomReturn(int localPlayerNr,
                                          const ExitGames::Common::Hashtable& roomProperties,
                                          const ExitGames::Common::Hashtable& playerProperties,
                                          int errorCode,
                                          const ExitGames::Common::JString& errorString)
        {
        }
        virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) {}
        virtual void joinLobbyReturn(void) {}
        virtual void leaveLobbyReturn(void) {}
        virtual void onFindFriendsResponse(void) {}

        // virtual void onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& /*lobbyStats*/) {}
        // virtual void webRpcReturn(int /*errorCode*/,
        //                           const ExitGames::Common::JString& /*errorString*/,
        //                           const ExitGames::Common::JString& /*uriPath*/,
        //                           int /*resultCode*/,
        //                           const ExitGames::Common::Dictionary<ExitGames::Common::Object, ExitGames::Common::Object>& /*returnData*/)
        // {
        // }

        // // info, that certain values have been updated
        // virtual void onRoomListUpdate(void) {}
        // virtual void onRoomPropertiesChange(const ExitGames::Common::Hashtable& /*changes*/) {}
        // virtual void onPlayerPropertiesChange(int /*playerNr*/, const ExitGames::Common::Hashtable& /*changes*/) {}
        // virtual void onAppStatsUpdate(void) {}
        // virtual void onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& /*lobbyStats*/) {}
        // virtual void onCacheSliceChanged(int /*cacheSliceIndex*/) {}
        // virtual void onMasterClientChanged(int id, int oldID) {}

        // // custom authentication
        // virtual void onCustomAuthenticationIntermediateStep(
        //     const ExitGames::Common::Dictionary<ExitGames::Common::JString, ExitGames::Common::Object>& /*customResponseData*/)
        // {
        // }

        // // receive the available server regions during the connect workflow (if you have specified in the constructor, that you want to select a region)
        // virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& /*availableRegions*/,
        //                                 const ExitGames::Common::JVector<ExitGames::Common::JString>& /*availableRegionServers*/)
        // {
        // }

        // virtual void onSecretReceival(const ExitGames::Common::JString& /*secret*/) {}

        // virtual void onDirectMessage(const ExitGames::Common::Object& /*msg*/, int /*remoteID*/, bool /*relay*/) {}

        // virtual void onCustomOperationResponse(const ExitGames::Photon::OperationResponse& /*operationResponse*/) {}

        // virtual void onGetRoomListResponse(
        //     const ExitGames::Common::JVector<ExitGames::Common::Helpers::SharedPointer<ExitGames::LoadBalancing::Room> >& /*roomList*/,
        //     const ExitGames::Common::JVector<ExitGames::Common::JString>& /*roomNameList*/)
        // {
        // }
    };
}  // namespace ComponentEngine::Photon
