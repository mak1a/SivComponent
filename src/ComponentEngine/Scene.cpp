#include "Scene.hpp"
namespace ComponentEngine
{
    void Scene::Update()
    {
        // Start呼び出し
        masterObject->components_start();

        masterObject->components_update();
        
        masterObject->components_lateUpdate();
        
        masterObject->components_draw();
    }
}  // namespace ComponentEngine
