#pragma once

#include <boost/noncopyable.hpp>
#include <list>
#include <memory>

#include "IComponent.hpp"
#include "Transform.hpp"

// for デプスステート
#include <map>
#include <vector>

namespace ComponentEngine
{
    using Transform = SivTransform2D;

    class IScene;

    class GameObject final : public std::enable_shared_from_this<GameObject>, private boost::noncopyable
    {
        friend class IScene;
        // public:
        //     using DrawCallStack = std::map<int, std::vector<std::shared_ptr<GameObject>>>;

    private:
        // GameObjectは必ずtransformを持つ
        Transform _transform;
        IScene* scene;

    public:
        IScene* GetScene()
        {
            return scene;
        }

    public:
        //プロパティ
        Transform& transform()
        {
            return _transform;
        }

    private:
        std::list<IComponent*> components;

        //親子オブジェクト
        std::shared_ptr<GameObject> parent;
        std::list<std::shared_ptr<GameObject>> children;

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
            : GameObject(Transform())
        {
        }

        explicit GameObject(const Transform& trans)
        {
            _transform = trans;
        }

        template <class Component, class... Args>
        Component* AddComponent(Args&&... args)
        {
            Component* c = new Component(std::forward<Args>(args)...);
            c->gameobject = weak_from_this();
            components.push_back(c);
            c->Awake();
            initializedAll = false;
            return c;
        }

        void AddChild(const std::shared_ptr<GameObject>& child)
        {
            child->SetParent(shared_from_this());
        }

        void SetParent(const std::shared_ptr<GameObject>& newParent)
        {
            if (newParent == nullptr)
            {
                return;
            }
            //自分を新しい親の子オブジェクトに設定
            newParent->children.push_back(shared_from_this());

            if (parent != nullptr)
            {
                //今の親と関係を解消
                parent->RemoveChild(shared_from_this());
            }

            //親を新しいオブジェクトに設定
            parent = newParent;
        }

        auto FindChild(const std::shared_ptr<GameObject>& child) -> decltype(std::find(children.begin(), children.end(), child))
        {
            return std::find(children.begin(), children.end(), child);
        }

        void DeleteChild(const std::shared_ptr<GameObject>& child)
        {
            auto itr = FindChild(child);
            if (itr == children.end())
            {
                return;
            }

            children.erase(itr);
        }

    private:
        bool RemoveChild(const std::shared_ptr<GameObject>& child)
        {
            auto itr = FindChild(child);

            if (itr == children.end())
            {
                return false;
            }

            children.erase(itr);
            return true;
        }

    public:
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

    public:
        virtual void components_start() final;

        virtual void components_update() final;

        virtual void components_lateUpdate() final;

        virtual void components_draw() const final;
    };

    template <>
    inline Transform* GameObject::GetComponent()
    {
        return &_transform;
    }
}  // namespace ComponentEngine

// https://qiita.com/kgwryk28/items/4a066d6f81ffacbc758f
