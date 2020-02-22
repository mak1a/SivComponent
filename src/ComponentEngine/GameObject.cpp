
#include "GameObject.hpp"


namespace ComponentEngine
{

void GameObject::Update()
{
    for(auto& component : components)
    {
        component->update_component();
    }
}

}
