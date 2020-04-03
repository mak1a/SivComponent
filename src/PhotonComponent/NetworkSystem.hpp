
#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>

#include "../ComponentEngine/SceneManager.hpp"
#include "LoadBalancing-cpp/inc/Client.h"

#include "AttachableComponentPhotonCallbacks.hpp"
#include "IObservable.hpp"

// #define PhotonDEBUG

namespace ComponentEngine::Photon
{
    std::string NetworkObjectName();

    class NetworkSystem : public ComponentEngine::AttachableComponent, public IPhotonObservable, public ExitGames::LoadBalancing::Listener
    {
        static NetworkSystem* instance;

        ExitGames::Common::JString playerName;

    public:
        NetworkSystem& SetPlayerName(const ExitGames::Common::JString& _playerName)
        {
            playerName = _playerName;
            return *this;
        }

        ExitGames::Common::JString GetPlayerName() const noexcept
        {
            return playerName;
        }

        static NetworkSystem* GetInstance()
        {
            return instance;
        }

        int GetServerTime() const
        {
            return mLoadBalancingClient.getServerTime();
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

    public:
        void createRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers)
        {
            mLoadBalancingClient.opJoinOrCreateRoom(roomName, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers));
        }

    public:
        using PlayerList = std::vector<ExitGames::LoadBalancing::Player*>;

    private:
        //プレイヤー情報の保持
        PlayerList playerList;

        void update_playerlist()
        {
            if (!mLoadBalancingClient.getIsInRoom())
            {
                playerList.clear();
                return;
            }

            auto p = mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers();
            auto len = p.getSize();
            playerList.resize(len);

            for (int k = 0; k < len; ++k)
            {
                playerList[k] = p[k];
            }

            std::sort(playerList.begin(), playerList.end(),
                      [](ExitGames::LoadBalancing::Player* a, ExitGames::LoadBalancing::Player* b) { return a->getNumber() < b->getNumber(); });
        }

    public:
        //        ExitGames::Common::JVector<ExitGames::LoadBalancing::Player*>&
        PlayerList& GetPlayerList()
        {
            return playerList;
        }

    private:
        std::list<AttachableComponentPhotonCallbacks*> observers;

    public:  // IPhotonObservable
        void Subscribe(ComponentEngine::Photon::AttachableComponentPhotonCallbacks* component) override
        {
            //多重登録へのチェックは面倒なのでなし！
            //ポインタで比較するか、購読側に登録済みか管理する機能をつければよい
            //そもそもコンストラクタでしか実行されない予定なので現状は不要なチェック
            observers.push_back(component);
            component->networkSystem = this;
        }

        void Dispose(ComponentEngine::Photon::AttachableComponentPhotonCallbacks* component) override
        {
            observers.remove(component);
        }

    private:
        ExitGames::LoadBalancing::Client mLoadBalancingClient;
        ExitGames::Common::Logger mLogger;  // accessed by EGLOG()

    public:
        ExitGames::LoadBalancing::Client& GetClient()
        {
            return mLoadBalancingClient;
        }

    public:
        ~NetworkSystem();

    private:
        // ExitGames::LoadBalancing::Listener

        // receive and print out debug out here
        virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override
        {
#ifdef PhotonDEBUG
            std::cout << "debugReturn" << std::endl;
#endif
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->debugReturn(debugLevel, string);
            }
        }

        // implement your error-handling here
        virtual void connectionErrorReturn(int errorCode) override
        {
#ifdef PhotonDEBUG
            std::cout << "connectionErrorReturn" << std::endl;
#endif
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->connectionErrorReturn(errorCode);
            }
        }
        virtual void clientErrorReturn(int errorCode) override
        {
#ifdef PhotonDEBUG
            std::cout << "clientErrorReturn" << std::endl;
#endif
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->clientErrorReturn(errorCode);
            }
        }
        virtual void warningReturn(int warningCode) override
        {
#ifdef PhotonDEBUG
            std::cout << "warningReturn" << std::endl;
#endif
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->warningReturn(warningCode);
            }
        }
        virtual void serverErrorReturn(int errorCode) override
        {
#ifdef PhotonDEBUG
            std::cout << "serverErrorReturn" << std::endl;
#endif
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
#ifdef PhotonDEBUG
            std::cout << "joinRoomEventAction" << std::endl;
#endif
            //プレイヤーリストを更新
            update_playerlist();

            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->joinRoomEventAction(playerNr, playernrs, player);
            }
        }
        virtual void leaveRoomEventAction(int playerNr, bool isInactive) override
        {
#ifdef PhotonDEBUG
            std::cout << "leaveRoomEventAction" << std::endl;
#endif
            //プレイヤーリストを更新
            update_playerlist();

            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->leaveRoomEventAction(playerNr, isInactive);
            }
        }

        virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override
        {
#ifdef PhotonDEBUG
            std::cout << "customEventAction" << std::endl;
#endif
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
#ifdef PhotonDEBUG
            std::cout << "connectReturn" << std::endl;
#endif

            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->connectReturn(errorCode, errorString, region, cluster);
            }
        }

        virtual void disconnectReturn(void) override
        {
#ifdef PhotonDEBUG
            std::cout << "disconnectReturn" << std::endl;
#endif
            update_playerlist();

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
#ifdef PhotonDEBUG
            std::cout << "createRoomReturn" << std::endl;
#endif
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
#ifdef PhotonDEBUG
            std::cout << "joinOrCreateRoomReturn" << std::endl;
#endif
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
#ifdef PhotonDEBUG
            std::cout << "joinRandomOrCreateRoomReturn" << std::endl;
#endif
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
#ifdef PhotonDEBUG
            std::cout << "joinRoomReturn" << std::endl;
#endif
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
#ifdef PhotonDEBUG
            std::cout << "joinRandomRoomReturn" << std::endl;
#endif
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->joinRandomRoomReturn(localPlayerNr, roomProperties, playerProperties, errorCode, errorString);
            }
        }
        virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override
        {
#ifdef PhotonDEBUG
            std::cout << "leaveRoomReturn" << std::endl;
#endif
            playerList.clear();

            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->leaveRoomReturn(errorCode, errorString);
            }
        }
        virtual void joinLobbyReturn(void) override
        {
#ifdef PhotonDEBUG
            std::cout << "joinLobbyReturn" << std::endl;
#endif
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->joinLobbyReturn();
            }
        }
        virtual void leaveLobbyReturn(void) override
        {
#ifdef PhotonDEBUG
            std::cout << "leaveLobbyReturn" << std::endl;
#endif
            for (AttachableComponentPhotonCallbacks* observer : observers)
            {
                observer->leaveLobbyReturn();
            }
        }
        virtual void onFindFriendsResponse(void) override
        {
#ifdef PhotonDEBUG
            std::cout << "onFindFriendsResponse" << std::endl;
#endif
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

    public:
        bool GetIsMasterClient()
        {
            if (playerList.size() == 0)
            {
                return false;
            }

            return playerList[0]->getNumber() == mLoadBalancingClient.getLocalPlayer().getNumber();
        }

        ///ルーム内でのプレイヤー番号の若さ(0から始まる)
        int GetPlayerNumberInRoom()
        {
            const auto pl = playerList.size();
            const auto myNumber = mLoadBalancingClient.getLocalPlayer().getNumber();
            for (int i = 0; i < pl; ++i)
            {
                if (playerList[i]->getNumber() == myNumber)
                {
                    return i;
                }
            }

            return -1;
        }

    private:
        // virtual void onMasterClientChanged(int id, int oldID)
        // {
        //     std::cout << "id" << id << " " << oldID << std::endl;
        //     isMasterClient = id == mLoadBalancingClient.getLocalPlayer().getUserID();

        //     for (AttachableComponentPhotonCallbacks* observer : observers)
        //     {
        //         observer->onMasterClientChanged(id, oldID);
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
}  // namespace ComponentEngine::Photon
