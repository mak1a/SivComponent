#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include <vector>

#include "AttachableComponent.hpp"
#include "Transform.hpp"
namespace ComponentEngine
{
    class GameObject : private boost::noncopyable
    {
    private:
        // GameObjectは必ずtransformを持つ
        Transform transform;

        std::vector<std::shared_ptr<AttachableComponent>> components;

        //親子オブジェクト
        std::shared_ptr<GameObject> parent;
        std::vector<std::shared_ptr<GameObject>> children;

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

        void AddComponent(std::shared_ptr<AttachableComponent> component) {
            components.push_back(component);
        }
        
        void Update();
    };
}  // namespace ComponentEngine
