
#pragma once

#include "AttachableComponent.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"

namespace ComponentEngine
{
    //    std::unique_ptr<ComponentEngine::GameObject>&& CreateGameObject();
    ComponentEngine::GameObject* CreateGameObject();

}  // namespace ComponentEngine
