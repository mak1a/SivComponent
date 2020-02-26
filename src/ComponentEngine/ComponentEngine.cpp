
#include "ComponentEngine.hpp"

namespace ComponentEngine
{
    std::shared_ptr<ComponentEngine::GameObject> CreateGameObject()
    {
        return std::make_shared<ComponentEngine::GameObject>();
    }
}  // namespace ComponentEngine
