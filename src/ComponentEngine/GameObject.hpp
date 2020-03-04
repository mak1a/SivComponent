#pragma once

#include <boost/noncopyable.hpp>
#include <list>
#include <memory>

#include "IComponent.hpp"
#include "Transform.hpp"

namespace ComponentEngine
{
    class GameObject final : private boost::noncopyable
    {
    public:
        using TransformType = SivTransform;

    private:
        // GameObjectは必ずtransformを持つ
        TransformType _transform;

    public:
        //プロパティ
        TransformType& transform()
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

        GameObject(const TransformType& trans)
        {
            _transform = trans;
        }

        template <class Component, class... Args>
        void AddComponent(Args&&... args)
        {
            IComponent* c = new Component(std::forward<Args>(args)...);
            c->gameobject = this;
            components.push_back(c);
            initializedAll = false;
            // return c;
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
        //エンジン用関数
        void Start();

        void Update();

        friend class Scene;
    };

    template <>
    inline GameObject::TransformType* GameObject::GetComponent()
    {
        return &_transform;
    }
}  // namespace ComponentEngine

// https://qiita.com/kgwryk28/items/4a066d6f81ffacbc758f