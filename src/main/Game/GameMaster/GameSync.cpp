#include "../../../PhotonComponent/NetworkSystem.hpp"
#include "GameSync.hpp"

void SendDefeatSignal()
{
    auto network = ComponentEngine::Photon::NetworkSystem::GetInstance();

    ExitGames::Common::Dictionary<nByte, int> dic;

    dic.put(DataName::GameSync::Result, static_cast<int>(DataName::GameSync::ResultType::Defeat));

    network->GetClient().opRaiseEvent(true, dic, CustomEvent::GameResult);
}

void SendVictorySignal()
{
    auto network = ComponentEngine::Photon::NetworkSystem::GetInstance();

    ExitGames::Common::Dictionary<nByte, int> dic;

    dic.put(DataName::GameSync::Result, static_cast<int>(DataName::GameSync::ResultType::Victory));

    network->GetClient().opRaiseEvent(true, dic, CustomEvent::GameResult);
}