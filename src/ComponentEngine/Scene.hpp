
#pragma once

#include <list>
#include "GameObject.hpp"
namespace ComponentEngine
{
    class Scene
    {
    private:
        std::list<GameObject*> objects;

    public:
        void AddObject(GameObject* obj)
        {
            //無効なポインタなら終了
            if (obj == nullptr)
            {
                return;
            }
            objects.push_back(obj);
        }
        
        void Update();

        virtual ~Scene()
        {
            //リソース開放
            for (GameObject* obj : objects)
            {
                delete obj;
            }
        }
    };
}  // namespace ComponentEngine
