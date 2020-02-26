
#pragma once

#include <vector>
#include "GameObject.hpp"
namespace ComponentEngine
{
    class Scene
    {
    private:
        std::vector<std::shared_ptr<GameObject>> objects;

    public:
        void AddObject(std::shared_ptr<GameObject> obj)
        {
            objects.emplace_back(obj);
            objects.back();
        }

        void Update();
    };
}  // namespace ComponentEngine
