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

        // GameObject(shared_ptr<GameObject> rval)
        // {
        //     _transform = std::move(rval->_transform);
        //     components = std::move(rval->components);
        //     parent = rval->parent;
        //     children = std::move(rval->children);
        // };

        template <class Component, class... Args>
        void AddComponent(Args&&... args)
        {
            IComponent* c = new Component(std::forward<Args>(args)...);
            c->Start();
            components.push_back(c);
            initializedAll = false;
            // return c;
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
}  // namespace ComponentEngine
