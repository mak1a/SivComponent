#include "Scene.hpp"
namespace ComponentEngine
{
    void Scene::Update()
    {
        // Start呼び出し
        for (auto& object : objects)
        {
            if (object->IsInitializedAll())
            {
                continue;
            }
            object->Start();
        }

        // Update
        for (auto& object : objects)
        {
            object->Update();
        }
    }
}  // namespace ComponentEngine
