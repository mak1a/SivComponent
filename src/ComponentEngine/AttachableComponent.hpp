
#pragma once

#include <memory>
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
        void SetActive(bool _active) override;
        bool GetActive() const override;

    public:
        std::weak_ptr<GameObject> GetGameObject() const;

    private:
        bool initialized = false;

    public:
        bool IsInitialized();

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
        AttachableComponent();

    protected:
        virtual void call_awake() override final
        {
            Awake();
        }

        // Startを1回しか呼ばないように制御
        // この関数はここで固定する
        virtual void call_start() override final;

        virtual bool _initialized() override final;

    public:
        virtual ~AttachableComponent() = default;
    };
}  // namespace ComponentEngine
