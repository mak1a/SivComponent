
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

    class IScene : public std::enable_shared_from_this<IScene>
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
            object->scene = weak_from_this();
        }

        [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetObject()
        {
            auto object = std::make_shared<GameObject>();
            object->scene = weak_from_this();
            masterObject->AddChild(object);
            return object;
        }

        [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetObject(const Transform& transform)
        {
            auto object = std::make_shared<GameObject>(transform);
            object->scene = weak_from_this();
            masterObject->AddChild(object);
            return object;
        }

    public:
        IScene()
            : isInitialized(false)
        {
            masterObject = std::make_shared<GameObject>();
            masterObject->SetName("MasterObject");
            masterObject->scene = weak_from_this();
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
            std::cout << "scene destory" << std::endl;
            std::cout << "master:" << masterObject.use_count() << std::endl;
            for (auto& c : masterObject->children)
            {
                std::cout << c->GetName() << ":" << c.use_count() << std::endl;
            }
        }
    };

}  // namespace ComponentEngine