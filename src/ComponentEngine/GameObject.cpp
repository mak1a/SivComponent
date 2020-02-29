
#include "GameObject.hpp"
#include "iostream"

namespace ComponentEngine
{
    void GameObject::Update()
    {
        for (auto& component : components)
        {
            component->Update();
        }
    }

    void GameObject::Start()
    {
        for (auto& component : components)
        {
            component->call_start();
        }
        initializedAll = true;
    }
}  // namespace ComponentEngine
