
#pragma once

#include <list>
#include "GameObject.hpp"
namespace ComponentEngine
{
    class Scene
    {
    private:
        //        std::list<GameObject*> objects;
        GameObject* masterObject;

    public:
        Scene()
        {
            masterObject = new GameObject();
        }

        void AddObject(GameObject* obj)
        {
            //無効なポインタなら終了
            if (obj == nullptr)
            {
                return;
            }
            masterObject->AddChild(obj);
        }

        void Update();

        virtual ~Scene()
        {
            delete masterObject;
        }
    };
}  // namespace ComponentEngine
