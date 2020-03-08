
#pragma once

#include "AttachableComponent.hpp"
#include "GameObject.hpp"
#include "SceneManager.hpp"

namespace ComponentEngine
{
    //    std::unique_ptr<ComponentEngine::GameObject>&& CreateGameObject();
    ComponentEngine::GameObject* CreateGameObject();

}  // namespace ComponentEngine
