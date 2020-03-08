
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

    class IScene
    {
    public:
        using KeyType = std::string;

    private:
        std::shared_ptr<GameObject> masterObject;

    private:
        bool isInitialized;

    public:
        bool IsInitialized() const noexcept
        {
            return isInitialized;
        }

    private:
        SceneManager* manager;
        friend SceneManager;

    public:
        SceneManager* GetSceneManager()
        {
            return manager;
        }

    protected:
        // void ChangeScene(const KeyType& sceneName) {}

    public:
        virtual void Setup() = 0;

        void AddObject(const std::shared_ptr<GameObject>& object)
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
            object->scene = this;
        }

        [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetObject()
        {
            auto object = std::make_shared<GameObject>();
            masterObject->AddChild(object);
            return object;
        }

        [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetObject(const Transform& transform)
        {
           auto object = std::make_shared<GameObject>(transform);
            masterObject->AddChild(object);
            return object;
        }

    public:
        IScene()
            : isInitialized(false)
        {
            masterObject = std::make_shared<GameObject>();
            masterObject->transform().SetName("MasterObject");
            masterObject->scene = this;
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

        virtual ~IScene()
        {
        }
    };

}  // namespace ComponentEngine
