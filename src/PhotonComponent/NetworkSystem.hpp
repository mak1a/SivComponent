
#pragma once

#include <iostream>
#include <list>
#include <memory>

#include "../ComponentEngine/SceneManager.hpp"
#include "LoadBalancing-cpp/inc/Client.h"

#include "AttachableComponentPhotonCallbacks.hpp"
#include "IObservable.hpp"

namespace PhotonComponent
{
    static const ExitGames::Common::JString PLAYER_NAME = L"user";

    std::string NetworkObjectName();

    class NetworkSystem : public ComponentEngine::AttachableComponent, public IPhotonObservable, public ExitGames::LoadBalancing::Listener
    {
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
        };

    private:
        States state;

    public:
        States GetState()
        {
            return state;
        }

        std::string GetStateString()
        {
            static std::unordered_map<States, std::string> dic;

            static bool once = [&]() {
                dic[States::INITIALIZED] = "INITIALIZED";
                dic[States::CONNECTING] = "CONNECTING";
                dic[States::CONNECTED] = "CONNECTED";
                dic[States::JOINING] = "JOINING";
                dic[States::JOINED] = "JOINED";
                dic[States::SENT_DATA] = "SENT_DATA";
                dic[States::RECEIVED_DATA] = "RECEIVED_DATA";
                dic[States::LEAVING] = "LEAVING";
                dic[States::LEFT] = "LEFT";
                dic[States::DISCONNECTING] = "DISCONNECTING";
                dic[States::DISCONNECTED] = "DISCONNECTED";
                return true;
            }();

            return dic[GetState()];
        }

    private:
        void SetState(States nextState)
        {
            std::cout << "state = " << GetStateString() << std::endl;
            state = nextState;
        }

    public:
        const ExitGames::Common::JString appID = L"b0f7b045-6e8c-4dd7-a671-e5aaeae5c2f4";  // set your app id here
        const ExitGames::Common::JString appVersion = L"1.0";

    public:
        NetworkSystem();
        void Connect(void);
        void Disconnect(void);
        void Update() override;
        void OnDestroy() override;

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
            //            auto eventhash = new ExitGames::Common::Hashtable();
            auto senddata = L"I am " + mLoadBalancingClient.getLocalPlayer().getName();
            mLoadBalancingClient.opRaiseEvent(true, senddata, 10);
        }

    private:
        std::list<AttachableComponentPhotonCallbacks*> observers;

    public:  // IPhotonObservable
        void Subscribe(PhotonComponent::AttachableComponentPhotonCallbacks* component) override
        {
            //多重登録へのチェックは面倒なのでなし！
            //ポインタで比較するか、購読側に登録済みか管理する機能をつければよい
            //そもそもコンストラクタでしか実行されない予定なので現状は不要なチェック
            observers.push_back(component);
        }

        void Dispose(PhotonComponent::AttachableComponentPhotonCallbacks* component) override
        {
            observers.remove(component);
        }

    private:
        ExitGames::LoadBalancing::Client mLoadBalancingClient;
        ExitGames::Common::Logger mLogger;  // accessed by EGLOG()

    public:
        ~NetworkSystem();

    private:  // ExitGames::LoadBalancing::Listener
              // void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override
              // {
              //     s3d::Print(U"debugReturn");
              //     s3d::Print(string.toString());
              // };
              // void connectionErrorReturn(int errorCode) override
              // {
              //     s3d::Print(U"connectionErrorReturn");
              // };
        // void clientErrorReturn(int errorCode) override
        // {
        //     s3d::Print(U"clientErrorReturn");
        // };

        // void warningReturn(int warningCode) override
        // {
        //     s3d::Print(U"warningReturn");
        // };

        // void serverErrorReturn(int errorCode) override
        // {
        //     s3d::Print(U"serverErrorReturn");
        // };

        // void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override
        // {
        //     s3d::Print(U"joinRoomEventAction");
        //     SetState(States::JOINED);
        // };

        // void leaveRoomEventAction(int playerNr, bool isInactive) override
        // {
        //     s3d::Print(U"leaveRoomEventAction");
        // };

        // void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override
        // {
        //     s3d::Print(U"customEventAction");

        //     if (eventCode == 10)
        //     {
        //         s3d::Print(ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).toString());
        //     }
        // };

        // void connectReturn(int errorCode,
        //                    const ExitGames::Common::JString& errorString,
        //                    const ExitGames::Common::JString& region,
        //                    const ExitGames::Common::JString& cluster) override
        // {
        //     if (errorCode)
        //     {
        //         s3d::Print(U"connect error");

        //         SetState(States::DISCONNECTING);
        //         return;
        //     }
        //     s3d::Print(U"connected!");
        //     SetState(States::CONNECTED);
        // };

        // void disconnectReturn() override
        // {
        //     s3d::Print(U"disconnectReturn");
        // };

        // void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override
        // {
        //     s3d::Print(U"leaveRoomReturn");
        // };

        // receive and print out debug out here
        virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->debugReturn(debugLevel, string);
            }
        }

        // implement your error-handling here
        virtual void connectionErrorReturn(int errorCode) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->connectionErrorReturn(errorCode);
            }
        }
        virtual void clientErrorReturn(int errorCode) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->clientErrorReturn(errorCode);
            }
        }
        virtual void warningReturn(int warningCode) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->warningReturn(warningCode);
            }
        }
        virtual void serverErrorReturn(int errorCode) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->serverErrorReturn(errorCode);
            }
        }

        // events, triggered by certain operations of all players in the same room
        virtual void joinRoomEventAction(int playerNr,
                                         const ExitGames::Common::JVector<int>& playernrs,
                                         const ExitGames::LoadBalancing::Player& player) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->joinRoomEventAction(playerNr, playernrs, player);
            }
        }
        virtual void leaveRoomEventAction(int playerNr, bool isInactive) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->leaveRoomEventAction(playerNr, isInactive);
            }
        }

        virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->customEventAction(playerNr, eventCode, eventContent);
            }
        }

        // callbacks for operations on the server
        virtual void connectReturn(int errorCode,
                                   const ExitGames::Common::JString& errorString,
                                   const ExitGames::Common::JString& region,
                                   const ExitGames::Common::JString& cluster) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->connectReturn(errorCode, errorString, region, cluster);
            }
        }
        virtual void disconnectReturn(void) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->disconnectReturn();
            }
        }
        virtual void createRoomReturn(int localPlayerNr,
                                      const ExitGames::Common::Hashtable& roomProperties,
                                      const ExitGames::Common::Hashtable& playerProperties,
                                      int errorCode,
                                      const ExitGames::Common::JString& errorString) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->createRoomReturn(localPlayerNr, roomProperties, playerProperties, errorCode, errorString);
            }
        }
        virtual void joinOrCreateRoomReturn(int localPlayerNr,
                                            const ExitGames::Common::Hashtable& roomProperties,
                                            const ExitGames::Common::Hashtable& playerProperties,
                                            int errorCode,
                                            const ExitGames::Common::JString& errorString) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->joinOrCreateRoomReturn(localPlayerNr, roomProperties, playerProperties, errorCode, errorString);
            }
        }
        virtual void joinRandomOrCreateRoomReturn(int localPlayerNr,
                                                  const ExitGames::Common::Hashtable& roomProperties,
                                                  const ExitGames::Common::Hashtable& playerProperties,
                                                  int errorCode,
                                                  const ExitGames::Common::JString& errorString) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->joinRandomOrCreateRoomReturn(localPlayerNr, roomProperties, playerProperties, errorCode, errorString);
            }
        }
        virtual void joinRoomReturn(int localPlayerNr,
                                    const ExitGames::Common::Hashtable& roomProperties,
                                    const ExitGames::Common::Hashtable& playerProperties,
                                    int errorCode,
                                    const ExitGames::Common::JString& errorString) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->joinRoomReturn(localPlayerNr, roomProperties, playerProperties, errorCode, errorString);
            }
        }
        virtual void joinRandomRoomReturn(int localPlayerNr,
                                          const ExitGames::Common::Hashtable& roomProperties,
                                          const ExitGames::Common::Hashtable& playerProperties,
                                          int errorCode,
                                          const ExitGames::Common::JString& errorString) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->joinRandomRoomReturn(localPlayerNr, roomProperties, playerProperties, errorCode, errorString);
            }
        }
        virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->leaveRoomReturn(errorCode, errorString);
            }
        }
        virtual void joinLobbyReturn(void) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->joinLobbyReturn();
            }
        }
        virtual void leaveLobbyReturn(void) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->leaveLobbyReturn();
            }
        }
        virtual void onFindFriendsResponse(void) override
        {
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->onFindFriendsResponse();
            }
        }

        // virtual void onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onLobbyStatsResponse();
        //     }
        // }
        // virtual void webRpcReturn(int /*errorCode*/,
        //                           const ExitGames::Common::JString& /*errorString*/,
        //                           const ExitGames::Common::JString& /*uriPath*/,
        //                           int /*resultCode*/,
        //                           const ExitGames::Common::Dictionary<ExitGames::Common::Object, ExitGames::Common::Object>& /*returnData*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->webRpcReturn();
        //     }
        // }

        // // info, that certain values have been updated
        // virtual void onRoomListUpdate(void)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onRoomListUpdate();
        //     }
        // }
        // virtual void onRoomPropertiesChange(const ExitGames::Common::Hashtable& /*changes*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onRoomPropertiesChange();
        //     }
        // }
        // virtual void onPlayerPropertiesChange(int /*playerNr*/, const ExitGames::Common::Hashtable& /*changes*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onPlayerPropertiesChange();
        //     }
        // }
        // virtual void onAppStatsUpdate(void)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onAppStatsUpdate();
        //     }
        // }
        // virtual void onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& /*lobbyStats*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onLobbyStatsUpdate();
        //     }
        // }
        // virtual void onCacheSliceChanged(int /*cacheSliceIndex*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onCacheSliceChanged();
        //     }
        // }
        // virtual void onMasterClientChanged(int /*id*/, int /*oldID*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onMasterClientChanged();
        //     }
        // }

        // // custom authentication
        // virtual void onCustomAuthenticationIntermediateStep(
        //     const ExitGames::Common::Dictionary<ExitGames::Common::JString, ExitGames::Common::Object>& /*customResponseData*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onCustomAuthenticationIntermediateStep();
        //     }
        // }

        // // receive the available server regions during the connect workflow (if you have specified in the constructor, that you want to select a region)
        // virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& /*availableRegions*/,
        //                                 const ExitGames::Common::JVector<ExitGames::Common::JString>& /*availableRegionServers*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onAvailableRegions();
        //     }
        // }

        // virtual void onSecretReceival(const ExitGames::Common::JString& /*secret*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onSecretReceival();
        //     }
        // }

        // virtual void onDirectMessage(const ExitGames::Common::Object& /*msg*/, int /*remoteID*/, bool /*relay*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onDirectMessage();
        //     }
        // }

        // virtual void onCustomOperationResponse(const ExitGames::Photon::OperationResponse& /*operationResponse*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onCustomOperationResponse();
        //     }
        // }

        // virtual void onGetRoomListResponse(
        //     const ExitGames::Common::JVector<ExitGames::Common::Helpers::SharedPointer<ExitGames::Common::Helper::Room> >& /*roomList*/,
        //     const ExitGames::Common::JVector<ExitGames::Common::JString>& /*roomNameList*/)
        // {
        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onGetRoomListResponse();
        //     }
        // }
    };
}  // namespace PhotonComponent
