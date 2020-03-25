
#pragma once

#include <list>

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

        bool isInitialized;

        SceneManager* manager = nullptr;

    public:
        bool IsInitialized() const noexcept
        {
            return isInitialized;
        }

        std::shared_ptr<GameObject> GetMasterObject()
        {
            return masterObject;
        }

        void _set_manager(SceneManager* m)
        {
            if (manager != nullptr)
            {
                return;
            }

            manager = m;
        }

        SceneManager* GetSceneManager()
        {
            return manager;
        }

        ComponentEngine::Collision::CollisionSystem& GetCollisionSystem()
        {
            return colsys;
        }

        //オブジェクト消去を予約
        void Destroy(std::weak_ptr<GameObject> object)
        {
            destroyList.push_back(object);
        }

        virtual void Setup() = 0;
        // virtual void EngineInit(ComponentEngine::SceneCommon&){};

        void AddObject(const std::shared_ptr<GameObject>& object)
        {
            //無効なポインタならエラー
            if (!object)
            {
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

        void Update()
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

            //衝突判定コール
            colsys.CollisionCall();

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

        IScene()
            : isInitialized(false)
        {
            masterObject = std::make_shared<GameObject>();
            masterObject->SetName("MasterObject");
            masterObject->scene = weak_from_this();
        }

        void DestoryAllObjects()
        {
            masterObject->DestroyAll();
        }

        virtual ~IScene() = default;
    };

}  // namespace ComponentEngine

//  bool IsInitialized() const noexcept;

//         SceneManager* GetSceneManager();

//         ComponentEngine::Collision::CollisionSystem& GetCollisionSystem();

//         //オブジェクト消去を予約
//         void Destroy(std::weak_ptr<GameObject> object);

//         virtual void Setup() = 0;

//         void AddObject(const std::shared_ptr<GameObject>& object);

//         [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetObject();

//         [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetObject(const Transform& transform);

//         void Update();

//         void DestoryAllObjects();

//         IScene();

//         virtual ~IScene() = default;
