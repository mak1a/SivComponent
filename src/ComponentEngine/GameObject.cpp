
#include "GameObject.hpp"
#include "iostream"

#include "AttachableComponent.hpp"

namespace ComponentEngine
{
    void GameObject::Update()
    {
        //変換行列を作成
        auto trans = _transform.PushTransform();
        
        for (IComponent* component : components)
        {
            component->Update();
        }

        for (GameObject* child : children)
        {
            child->Update();
        }
    }

    void GameObject::Start()
    {
        for (IComponent* component : components)
        {
            component->call_start();
        }
        initializedAll = true;

        for (GameObject* child : children)
        {
            // TODO: Transformの処理
            child->Start();
        }
    }
}  // namespace ComponentEngine
