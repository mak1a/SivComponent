#pragma once

#include <list>

#include <boost/noncopyable.hpp>

#include "AttachableComponent.hpp"

#include "../SivComponent/Collision/CollisionSystem.hpp"

namespace ComponentEngine
{
    class SceneManager;
    class GameObject;

    class IScene : boost::noncopyable  //, public std::enable_shared_from_this<IScene>
    {
        friend SceneManager;

    public:
        using KeyType = std::string;

    private:
        ComponentEngine::Collision::CollisionSystem colsys;

        std::shared_ptr<GameObject> masterObject;

        std::list<std::weak_ptr<GameObject>> destroyList;

        bool isInitialized;

        SceneManager* manager;

    public:
        void x();

        bool IsInitialized() const noexcept;

        std::shared_ptr<GameObject> GetMasterObject();

        SceneManager* GetSceneManager();

        ComponentEngine::Collision::CollisionSystem& GetCollisionSystem();

        //オブジェクト消去を予約
        void Destroy(std::weak_ptr<GameObject> object);

        virtual void Setup() = 0;

        void AddObject(const std::shared_ptr<GameObject>& object);

        [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetObject();

        [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetObject(const Transform& transform);

        void Update();

        void DestoryAllObjects();

        IScene();

        virtual ~IScene() = default;
    };

}  // namespace ComponentEngine
