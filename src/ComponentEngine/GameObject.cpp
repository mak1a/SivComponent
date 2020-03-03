
#include "GameObject.hpp"
#include "iostream"

namespace ComponentEngine
{
    void GameObject::Update()
    {
        for (auto component : components)
        {
            component->Update();
        }

        for (auto child : children)
        {
            child->Update();
        }
    }

    void GameObject::Start()
    {
        for (auto& component : components)
        {
            component->call_start();
        }
        initializedAll = true;

        for (auto child : children)
        {
            child->Start();
        }
    }
}  // namespace ComponentEngine
