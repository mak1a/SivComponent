
#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include "GameObject.hpp"
#include "IComponent.hpp"

namespace ComponentEngine
{
    //全てのコンポーネントの基本
    class AttachableComponent : public IComponent
    {
        friend class GameObject;

    protected:
        std::weak_ptr<GameObject> gameObject()
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
        virtual void Awake() override {}
        virtual void Start() {}
        virtual void Update() override {}
        virtual void LateUpdate() override {}
        virtual void Draw() const override {}

        //これ自体の生成は禁止する
        AttachableComponent(){};

    protected:
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

    public:
        virtual ~AttachableComponent() = default;
    };
}  // namespace ComponentEngine
