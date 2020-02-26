#include "Scene.hpp"

namespace ComponentEngine
{
    void Scene::Update()
    {
        for (auto& object : objects)
        {
            if (!object->IsInitializedAll())
            {
                object->Start();
            }
        }

        for (auto& object : objects)
        {
            object->Update();
        }
    }
}  // namespace ComponentEngine
