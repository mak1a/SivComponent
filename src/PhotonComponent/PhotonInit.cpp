#include "PhotonInit.hpp"

#include "LoadBalancing-cpp/inc/Client.h"
#include "NetworkSystem.hpp"

namespace ComponentEngine::Photon
{
    void PhotonInit(ComponentEngine::SceneManager& manager)
    {
        auto photon = manager.GetCommon().CreateAndGetCommonObject(NetworkObjectName());
        photon->AddComponent<NetworkSystem>();
    }
}  // namespace ComponentEngine::Photon
