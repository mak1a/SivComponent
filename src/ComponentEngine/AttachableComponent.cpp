#include "AttachableComponent.hpp"

namespace ComponentEngine
{
    void AttachableComponent::SetActive(bool _active)
    {
        active = _active;
    }
    bool AttachableComponent::GetActive() const
    {
        return active;
    }

    std::weak_ptr<GameObject> AttachableComponent::GetGameObject() const
    {
        return gameobject;
    }

    bool AttachableComponent::IsInitialized()
    {
        return initialized;
    }

    //これ自体の生成は禁止する
    AttachableComponent::AttachableComponent()
    {
        active = true;
    };

    // Startを1回しか呼ばないように制御
    // この関数はここで固定する
    void AttachableComponent::call_start()
    {
        if (!initialized)
        {
            Start();
            initialized = true;
        }
    }

    bool AttachableComponent::_initialized()
    {
        return initialized;
    }

}  // namespace ComponentEngine