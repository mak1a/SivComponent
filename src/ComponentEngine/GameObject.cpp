
#include "GameObject.hpp"

namespace ComponentEngine
{
    void GameObject::Update()
    {
        for (auto& component : components)
        {
            component->call_start();
        }
    }

    void GameObject::Start()
    {
        for (auto& component : components)
        {
            component->Start();
        }
        initializedAll = true;
    }

}  // namespace ComponentEngine
