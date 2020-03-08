
#pragma once

#include <list>

#ifdef DEBUG
#include <iostream>
#endif

#include "AttachableComponent.hpp"
#include "GameObject.hpp"
namespace ComponentEngine
{
    class SceneManager;

    class Scene
    {
    private:
        GameObject* masterObject;

    private:
        bool isInitialized;

    public:
        bool IsInitialized() const noexcept
        {
            return isInitialized;
        }

    protected:
        SceneManager* manager;
        friend SceneManager;

    public:
        virtual void Setup() = 0;

        void AddObject(GameObject* object)
        {
            //無効なポインタならエラー
            if (object == nullptr)
            {
#ifdef DEBUG
                std::cout << "Object Pointer is Null." << std::endl;
#endif
                return;
            }
            masterObject->AddChild(object);
        }

        [[nodiscard]] GameObject* CreateAndGetObject()
        {
            GameObject* object = new GameObject();
            masterObject->AddChild(object);
            return object;
        }

        [[nodiscard]] GameObject* CreateAndGetObject(const Transform& transform)
        {
            GameObject* object = new GameObject(transform);
            masterObject->AddChild(object);
            return object;
        }

    public:
        Scene()
            : isInitialized(false)
        {
            masterObject = new GameObject();
        }

    public:
        virtual void Update() final
        {
            if (!isInitialized)
            {
                Setup();
                isInitialized = true;
            }

            // Start呼び出し
            masterObject->components_start();

            masterObject->components_update();

            masterObject->components_lateUpdate();

            masterObject->components_draw();
        }

        virtual ~Scene()
        {
            delete masterObject;
        }
    };

}  // namespace ComponentEngine
