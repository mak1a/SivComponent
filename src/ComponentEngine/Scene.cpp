#include "Scene.hpp"
namespace ComponentEngine
{
    void Scene::Update()
    {
        // Start呼び出し
        masterObject->Start();

        masterObject->Update();
    }
}  // namespace ComponentEngine
