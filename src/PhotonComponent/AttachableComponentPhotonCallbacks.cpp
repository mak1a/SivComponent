
#include "AttachableComponentPhotonCallbacks.hpp"
#include "NetworkSystem.hpp"

namespace PhotonComponent
{
    void AttachableComponentPhotonCallbacks::Awake()
    {
        //自身をコールバック対象に設定する
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommonObject(NetworkObjectName());
        object->GetComponent<NetworkSystem>()->Subscribe(this);
    }

    void AttachableComponentPhotonCallbacks::OnDestroy()
    {
        //コールバック対象から外れる
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommonObject(NetworkObjectName());
        object->GetComponent<NetworkSystem>()->Dispose(this);
    }
}  // namespace PhotonComponent
