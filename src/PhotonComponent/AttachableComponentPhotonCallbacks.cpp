
#include "AttachableComponentPhotonCallbacks.hpp"
#include "NetworkSystem.hpp"

namespace ComponentEngine::Photon
{
    void AttachableComponentPhotonCallbacks::Awake()
    {
        //自身をコールバック対象に設定する
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetCommonObject(NetworkObjectName());
        object->GetComponent<NetworkSystem>()->Subscribe(this);
    }

    void AttachableComponentPhotonCallbacks::OnDestroy()
    {
        //コールバック対象から外れる
        auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetCommonObject(NetworkObjectName());
        object->GetComponent<NetworkSystem>()->Dispose(this);
        OnDestory2();
    }

}  // namespace ComponentEngine::Photon
