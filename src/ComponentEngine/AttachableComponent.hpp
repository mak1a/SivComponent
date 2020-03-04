
#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include "GameObject.hpp"
#include "IComponent.hpp"

namespace ComponentEngine
{
    class AttachableComponent : public IComponent
    {
        friend class GameObject;

    private:
        // GameObjectとのやりとり
        GameObject* gameobject;

        // template <class Component, class... Args>
        // friend Component* GameObject::AddComponent(Args&&... args);
        // friend class GameObject;
    private:

    protected:
        //委譲API
        GameObject& gameObject()
        {
            return *gameobject;
        }

    private:
        bool initialized = false;

    public:
        bool IsInitialized()
        {
            return initialized;
        }

    protected:
        // virtual funcs
        virtual void Start() override {}
        virtual void Update() override {}

        //これ自体の生成を禁止するためprotected
        AttachableComponent(){};

    protected:
        void call_start() override
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
