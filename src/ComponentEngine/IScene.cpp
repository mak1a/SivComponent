#include "IScene.hpp"

#include "GameObject.hpp"

namespace ComponentEngine
{
    // bool IScene::IsInitialized() const noexcept
    // {
    //     return isInitialized;
    // }

    // SceneManager* IScene::GetSceneManager()
    // {
    //     return manager;
    // }

    // ComponentEngine::Collision::CollisionSystem& IScene::GetCollisionSystem()
    // {
    //     return colsys;
    // }

    // //オブジェクト消去を予約
    // void IScene::Destroy(std::weak_ptr<GameObject> object)
    // {
    //     destroyList.push_back(object);
    // }

    // void IScene::AddObject(const std::shared_ptr<GameObject>& object)
    // {
    //     //無効なポインタならエラー
    //     if (!object)
    //     {
    //         return;
    //     }
    //     masterObject->AddChild(object);
    //     // object->scene = weak_from_this();
    // }
    // std::shared_ptr<GameObject> IScene::CreateAndGetObject()
    // {
    //     auto object = std::make_shared<GameObject>();
    //     // object->scene = weak_from_this();
    //     masterObject->AddChild(object);
    //     return object;
    // }

    // std::shared_ptr<GameObject> IScene::CreateAndGetObject(const Transform& transform)
    // {
    //     auto object = std::make_shared<GameObject>(transform);
    //     object->scene = weak_from_this();
    //     masterObject->AddChild(object);
    //     return object;
    // }

    // void IScene::Update()
    // {
    //     if (!isInitialized)
    //     {
    //         Setup();
    //         isInitialized = true;
    //     }

    //     // Start呼び出し
    //     masterObject->components_start();

    //     masterObject->components_update();

    //     masterObject->components_lateUpdate();

    //     //衝突判定コール
    //     colsys.CollisionCall();

    //     masterObject->components_draw();

    //     //消去処理
    //     for (auto& obj : destroyList)
    //     {
    //         auto o = obj.lock();
    //         if (!o)
    //         {
    //             continue;
    //         }

    //         o->parent.lock()->DeleteChild(o);
    //     }
    // }

    // IScene::IScene()
    //     : isInitialized(false)
    // {
    //     masterObject = std::make_shared<GameObject>();
    //     masterObject->SetName("MasterObject");
    //     // masterObject->scene = weak_from_this();
    // }

    // void IScene::DestoryAllObjects()
    // {
    //     masterObject->DestroyAll();
    // }
}  // namespace ComponentEngine
