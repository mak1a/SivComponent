#pragma once

#include <boost/noncopyable.hpp>
#include <list>
#include <memory>

#include "IComponent.hpp"
#include "Transform.hpp"

namespace ComponentEngine
{
    using Transform = SivTransform2D;

    class GameObject final : private boost::noncopyable
    {
    private:
        // GameObjectは必ずtransformを持つ
        Transform _transform;

    public:
        //プロパティ
        Transform& transform()
        {
            return _transform;
        }

    private:
        std::list<IComponent*> components;

        //親子オブジェクト
        GameObject* parent;
        std::list<GameObject*> children;

    private:
        // Start更新済みかどうか
        bool initializedAll = false;

    public:
        bool IsInitializedAll()
        {
            return initializedAll;
        }

    public:
        GameObject()
            : _transform()
        {
        }

        GameObject(const Transform& trans)
        {
            _transform = trans;
        }

        template <class Component, class... Args>
        Component* AddComponent(Args&&... args)
        {
            Component* c = new Component(std::forward<Args>(args)...);
            c->gameobject = this;
            components.push_back(c);
            c->Awake();
            initializedAll = false;
            return c;
        }

        void AddChild(GameObject* child)
        {
            children.push_back(child);
        }

        void DeleteChild(GameObject* child)
        {
            auto itr = std::find(children.begin(), children.end(), child);
            if (itr == children.end())
            {
                return;
            }

            delete *itr;
            children.erase(itr);
        }

        template <class T>
        T* GetComponent()
        {
            T* component;

            for (IComponent* c : components)
            {
                if ((component = dynamic_cast<T*>(c)))
                {
                    return component;
                }
            }
            return nullptr;
        }

        ~GameObject()
        {
            for (IComponent* component : components)
            {
                // TODO: OnDestory
                delete component;
            }
        }

    private:
        void components_start();

        void components_update();

        void components_lateUpdate();

        void components_draw() const;

        friend class Scene;
    };

    template <>
    inline Transform* GameObject::GetComponent()
    {
        return &_transform;
    }
}  // namespace ComponentEngine

// https://qiita.com/kgwryk28/items/4a066d6f81ffacbc758f
