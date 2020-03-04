
#include "GameObject.hpp"
#include "iostream"

#include "AttachableComponent.hpp"

namespace ComponentEngine
{
    void GameObject::Update()
    {
        for (IComponent* component : components)
        {
            component->Update();
        }

        for (GameObject* child : children)
        {
            // s3d::Transformer2D
            auto transforer = child->_transform.PushTransform();
            // TODO: Transformの処理
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
