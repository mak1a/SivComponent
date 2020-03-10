
#include "AttachableComponentPhotonCallbacks.hpp"
#include "NetworkSystem.hpp"

namespace PhotonComponent
{
    void AttachableComponentPhotonCallbacks::Awake()
    {
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommonObject(NetworkObjectName());
        object->GetComponent<NetworkSystem>()->Subscribe(this);
    }

    void AttachableComponentPhotonCallbacks::OnDestroy()
    {
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommonObject(NetworkObjectName());
        object->GetComponent<NetworkSystem>()->Dispose(this);
    }
}  // namespace PhotonComponent
