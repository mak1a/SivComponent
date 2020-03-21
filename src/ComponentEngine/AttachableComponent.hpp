
#pragma once

#define NO_S3D_USING
#include <memory>
// #include "GameObject.hpp"
#include "IComponent.hpp"

namespace ComponentEngine
{
    class GameObject;

    //全てのコンポーネントの基本
    class AttachableComponent : public IComponent
    {
        friend class GameObject;

    private:
        bool active;

    public:
        void SetActive(bool _active) override
        {
            active = _active;
        }
        bool GetActive() const override
        {
            return active;
        }

    public:
        std::weak_ptr<GameObject> GetGameObject()
        {
            return gameobject;
        }

        //        auto GetScene() -> decltype(gameobject.lock()->GetScene())
        //        {
        //            return gameobject.lock()->GetScene();
        //        }

        //        auto transform() -> decltype(gameobject.lock()->transform())
        //        {
        //            return gameobject.lock()->transform();
        //        }

        //        SceneManager* GetSceneManager()
        //        {
        //            return gameobject->
        //        }

    private:
        bool initialized = false;

    public:
        bool IsInitialized()
        {
            return initialized;
        }

    protected:
        // virtual funcs
        virtual void Awake() {}
        virtual void Start() {}
        virtual void Update() override {}
        virtual void LateUpdate() override {}
        virtual void Draw() const override {}
        virtual void OnDestroy() override {}
        virtual void OnStayCollision(std::shared_ptr<GameObject>&) override {}

        //これ自体の生成は禁止する
        AttachableComponent()
        {
            active = true;
        };

    protected:
        virtual void call_awake() override final
        {
            Awake();
        }

        // Startを1回しか呼ばないように制御
        // この関数はここで固定する
        virtual void call_start() override final
        {
            if (!initialized)
            {
                Start();
                initialized = true;
            }
        }

        virtual bool _initialized() override final
        {
            return initialized;
        }

    public:
        virtual ~AttachableComponent() = default;
    };
}  // namespace ComponentEngine
