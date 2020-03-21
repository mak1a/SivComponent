
#pragma once

#include <list>

#ifdef DEBUG
#include <iostream>
#endif

#include <boost/noncopyable.hpp>

#include "AttachableComponent.hpp"
#include "GameObject.hpp"

#include "../SivComponent/Collision/CollisionSystem.hpp"

namespace ComponentEngine
{
    class SceneManager;

    class IScene : public std::enable_shared_from_this<IScene>, boost::noncopyable
    {
    public:
        using KeyType = std::string;

    private:
        ComponentEngine::Collision::CollisionSystem colsys;

        std::shared_ptr<GameObject> masterObject;

        std::list<std::weak_ptr<GameObject>> destroyList;

    public:
        ComponentEngine::Collision::CollisionSystem& GetCollisionSystem()
        {
            return colsys;
        }

        //オブジェクト消去を予約
        void Destroy(std::weak_ptr<GameObject> object)
        {
            destroyList.push_back(object);
        }

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
            if (!object)
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

            colsys.CollisionCall();

            masterObject->components_update();

            masterObject->components_lateUpdate();

            masterObject->components_draw();

            //消去処理
            for (auto& obj : destroyList)
            {
                auto o = obj.lock();
                if (!o)
                {
                    continue;
                }

                o->parent.lock()->DeleteChild(o);
            }
        }

        void DestoryAllObjects()
        {
            masterObject->DestroyAll();
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
