#pragma once

#define NO_S3D_USING
#include <boost/noncopyable.hpp>
#include <list>
#include <memory>

#include "IComponent.hpp"
#include "Transform.hpp"

namespace ComponentEngine
{
    using Transform = SivTransform2D;

    class IScene;

    class GameObject final : public std::enable_shared_from_this<GameObject>, private boost::noncopyable
    {
        friend class IScene;
        // public:
        //     using DrawCallStack = std::map<int, std::vector<std::shared_ptr<GameObject>>>;

    public:
        // GameObjectは必ずtransformを持つ
        Transform _transform;
        std::weak_ptr<IScene> scene;

    private:
        std::string name = "unnamed";

    public:
        GameObject& SetName(const std::string& _name)
        {
            name = _name;
            return *this;
        }

        std::string GetName() const noexcept
        {
            return name;
        }

    public:
        std::weak_ptr<IScene> GetScene()
        {
            return scene;
        }

    private:
        bool active;

    public:
        GameObject& SetActive(bool _active)
        {
            active = _active;
            for (const auto& c : children)
            {
                c->SetActive(_active);
            }
            return *this;
        }

        bool GetActive() const noexcept
        {
            return active;
        }

    public:
        //プロパティ
        Transform& transform()
        {
            return _transform;
        }

    private:
        std::list<std::shared_ptr<IComponent>> components;

        //親子オブジェクト
        std::weak_ptr<GameObject> parent;
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
            active = true;
            _transform = trans;
        }

        template <class Component, class... Args>
        std::shared_ptr<Component> AddComponent(Args&&... args)
        {
            auto c = std::make_shared<Component>(std::forward<Args>(args)...);
            c->gameobject = weak_from_this();
            // std::cout << c->gameobject.lock()->GetName() << std::endl;
            components.push_back(c);

            c->call_awake();
            initializedAll = false;
            return c;
        }

        void AddChild(const std::shared_ptr<GameObject>& child)
        {
            child->SetParent(weak_from_this());
        }

        [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetChild()
        {
            auto object = std::make_shared<GameObject>();
            object->scene = this->scene;
            this->AddChild(object);
            return object;
        }

        void SetParent(const std::weak_ptr<GameObject>& newParent)
        {
            //現在の関係をデバッグ出力
            // std::cout << "this is:" << GetName() << std::endl;

            if (!newParent.lock())
            {
                return;
            }
            //自分を新しい親の子オブジェクトに設定
            newParent.lock()->children.push_back(shared_from_this());

            if (parent.lock())
            {
                // DEBUG
                // std::cout << "parent is:" << parent.lock()->GetName() << std::endl;
                //今の親と関係を解消
                parent.lock()->RemoveChild(shared_from_this());
            }

            //親を新しいオブジェクトに設定
            parent = newParent;
            // DEBUG
            // std::cout << "new parent is:" << parent.lock()->GetName() << "\n" << std::endl;
        }

    private:
        auto FindChildItr(const std::shared_ptr<GameObject>& child) const -> decltype(std::find(children.begin(), children.end(), child))
        {
            return std::find(children.begin(), children.end(), child);
        }

        auto FindChildItr(const std::string& name) const -> decltype(std::find(children.begin(), children.end(), children.front()))
        {
            return std::find_if(children.begin(), children.end(), [&](const std::shared_ptr<GameObject>& obj) { return obj->GetName() == name; });
        }

    public:
        std::shared_ptr<GameObject> FindChild(const std::shared_ptr<GameObject>& child) const
        {
            auto itr = FindChildItr(child);

            if (itr == children.end())
            {
                return std::shared_ptr<GameObject>();
            }

            return *itr;
        }

        std::shared_ptr<GameObject> FindChild(const std::string& name) const
        {
            auto itr = std::find_if(children.begin(), children.end(), [&](const std::shared_ptr<GameObject>& obj) { return obj->GetName() == name; });

            if (itr == children.end())
            {
                return std::shared_ptr<GameObject>();
            }

            return *itr;
        }

        void DeleteChild(const std::shared_ptr<GameObject>& child)
        {
            auto itr = FindChildItr(child);
            if (itr == children.end())
            {
                return;
            }

            child->DestroyAllChildren();

            children.erase(itr);
        }

    private:
        //子オブジェクトを削除せずに外します
        bool RemoveChild(const std::shared_ptr<GameObject>& child)
        {
            auto itr = FindChildItr(child);

            if (itr == children.end())
            {
                return false;
            }

            children.erase(itr);
            return true;
        }

    public:
        template <class T>
        std::shared_ptr<T> GetComponent() const
        {
            std::shared_ptr<T> component;

            for (const auto& c : components)
            {
                if ((component = std::dynamic_pointer_cast<T>(c)))
                {
                    return component;
                }
            }
            return nullptr;
        }

        template <class T>
        std::weak_ptr<T> GetComponentWeak() const
        {
            std::weak_ptr<T> component;
            for (const auto& c : components)
            {
                if ((component = std::dynamic_pointer_cast<T>(c)))
                {
                    return component;
                }
            }
            return std::weak_ptr<T>();
        }

        //デストラクタに任せると伝播方向の関係でポインタが無効になって死ぬので先に呼べ
        //すべての子オブジェクトを破壊する
        void DestroyAll()
        {
            if (GetName() == "MasterObject")
            {
                auto itr = FindChildItr("CommonParent");
                if (itr != children.end())
                {
                    children.erase(itr);
                }
            }

            //まずはすべてのコンポーネントのOnDestoryを呼び出す
            DestroyAllChildrenComponents();
            //次にすべての子オブジェクトを削除
            DestroyAllChildren();
        }

    private:
        void DestroyAllChildren()
        {
            for (const auto& child : children)
            {
                child->DestroyAllChildren();
            }

            children.clear();
        }

        void DestroyAllChildrenComponents()
        {
            for (const auto& component : components)
            {
                component->OnDestroy();
            }
            for (const auto& child : children)
            {
                child->DestroyAllChildrenComponents();
            }
        }

    public:
        ~GameObject()
        {
            //            DestoryAllObjects();
            // DEBUG
            // std::cout << "Destory:" << GetName() << std::endl;
        }

    public:
        virtual void components_start() final;

        virtual void components_update() final;

        virtual void components_lateUpdate() final;

        virtual void components_draw() const final;
    };  // namespace ComponentEngine
}  // namespace ComponentEngine
