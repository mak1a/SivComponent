
#include "GameSync.hpp"
#include "../../../PhotonComponent/NetworkSystem.hpp"

// using namespace DataName::GameSync;

void SendDefeatSignal()
{
    auto network = ComponentEngine::Photon::NetworkSystem::GetInstance();

    ExitGames::Common::Dictionary<nByte, int> dic;

    dic.put(DataName::GameSync::Result, static_cast<int>(DataName::GameSync::ResultType::Defeat));

    // PlayerManagerが受け取る
    network->GetClient().opRaiseEvent(true, dic, CustomEvent::GameResult);
}
void SendVictorySignal()
{
    auto network = ComponentEngine::Photon::NetworkSystem::GetInstance();

    ExitGames::Common::Dictionary<nByte, int> dic;

    dic.put(DataName::GameSync::Result, static_cast<int>(DataName::GameSync::ResultType::Victory));

    // PlayerManagerが受け取る
    network->GetClient().opRaiseEvent(true, dic, CustomEvent::GameResult);
}