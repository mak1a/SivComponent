#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include <vector>

#include "IComponent.hpp"
#include "Transform.hpp"

namespace ComponentEngine
{
    class GameObject
    {
    private:
        // GameObjectは必ずtransformを持つ
        Transform transform;
        
    private:

        std::vector<std::shared_ptr<IComponent>> components;

        //親子オブジェクト
        std::shared_ptr<GameObject> parent;
        std::vector<std::shared_ptr<GameObject>> children;

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
            : transform()
        {
        }

        GameObject(const Transform& _transform)
        {
            transform = _transform;
        }

        GameObject(std::shared_ptr<GameObject> rval)
        {
            transform = std::move(rval->transform);
            components = std::move(rval->components);
            parent = rval->parent;
            children = std::move(rval->children);
        };

        void AddComponent(std::shared_ptr<IComponent> component)
        {
            components.push_back(component);
            initializedAll = false;
        }

        void Start();

        void Update();
    };
}  // namespace ComponentEngine
