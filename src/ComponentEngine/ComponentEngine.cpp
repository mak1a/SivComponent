
#include "ComponentEngine.hpp"

namespace ComponentEngine
{
    //    std::unique_ptr<ComponentEngine::GameObject>&& CreateGameObject()
    ComponentEngine::GameObject* CreateGameObject()
    {
        return new GameObject();
        //        return std::make_unique<ComponentEngine::GameObject>();
    }
}  // namespace ComponentEngine
