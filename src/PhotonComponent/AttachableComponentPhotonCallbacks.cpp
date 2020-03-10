
#include "AttachableComponentPhotonCallbacks.hpp"
#include "NetworkSystem.hpp"

namespace PhotonComponent
{
    AttachableComponentPhotonCallbacks::AttachableComponentPhotonCallbacks()
    {
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommonObject(NetworkObjectName());
        object->GetComponent<NetworkSystem>()->Subscribe(this);
    }

    AttachableComponentPhotonCallbacks::~AttachableComponentPhotonCallbacks()
    {
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommonObject(NetworkObjectName());
        object->GetComponent<NetworkSystem>()->Dispose(this);
    }
}  // namespace PhotonComponent
