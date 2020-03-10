
#pragma once

#include <iostream>
#include <list>
#include <memory>

#include "LoadBalancing-cpp/inc/Client.h"
// #include "ComponentEngine.hpp"
#include "../ComponentEngine/SceneManager.hpp"

#include "IObservable.hpp"
#include "Listener.hpp"

namespace PhotonComponent
{
    static const ExitGames::Common::JString PLAYER_NAME = L"user";

    std::string NetworkObjectName();

    class NetworkSystem : public ComponentEngine::AttachableComponent, public IPhotonObservable
    {
    public:
        const ExitGames::Common::JString appID = L"b0f7b045-6e8c-4dd7-a671-e5aaeae5c2f4";  // set your app id here
        const ExitGames::Common::JString appVersion = L"1.0";

    public:
        NetworkSystem();
        void Connect(void);
        void Disconnect(void);
        void Update() override;

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
        // 今後のshared_ptrへの差し替えを考慮
        std::list<AttachableComponentPhotonCallbacks*> observers;

    public:
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
        PhotonListener mListener;           // your implementation of the ExitGames::LoadBalancing::Listener interface
        ExitGames::Common::Logger mLogger;  // accessed by EGLOG()

    public:
        ~NetworkSystem();
    };
}  // namespace PhotonComponent
