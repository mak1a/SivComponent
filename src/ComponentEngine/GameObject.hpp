#pragma once

#define NO_S3D_USING
#include <boost/noncopyable.hpp>
#include <list>
#include <memory>

#include "IComponent.hpp"
#include "Transform.hpp"

#include "../SivComponent/Collision/CollisionSystem.hpp"

namespace ComponentEngine
{
    using Transform = SivTransform2D;

    class IScene;

    class GameObject final : public std::enable_shared_from_this<GameObject>, private boost::noncopyable
    {
        friend class IScene;
        // public:
        //     using DrawCallStack = std::map<int, std::vector<std::shared_ptr<GameObject>>>;
        
        ComponentEngine::Collision::CollisionSystem colsys;

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

        std::weak_ptr<IScene> GetScene()
        {
            // sceneの参照を持ってなかったら親から回収する
            if (scene.expired())
            {
                scene = parent.lock()->GetScene();
            }

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
        // weak_ptrを使えるようにするためにshared_ptrにしたが、正直生ポインタでいいところではある。
        //メモリリークのリスクの方が大きい気もする。
        std::list<std::shared_ptr<IComponent>> components;

        //親子オブジェクト
        std::weak_ptr<GameObject> parent;
        std::list<std::shared_ptr<GameObject>> children;

    public:
        [[nodiscard]] std::weak_ptr<GameObject>& GetParent()
        {
            return parent;
        }

        [[nodiscard]] std::list<std::shared_ptr<GameObject>>& GetChildren()
        {
            return children;
        }

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

        //実際に動くコンストラクタ
        explicit GameObject(const Transform& trans)
        {
            active = true;
            _transform = trans;
        }

        // make_shared書くの面倒なのでね
        static std::shared_ptr<GameObject> Create()
        {
            return std::make_shared<GameObject>();
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

        ///削除成功したらtrue
        template <class Component>
        bool DeleteComponent()
        {
            auto end = components.end();

            for (auto c = components.begin(); c != end; ++c)
            {
                if (std::dynamic_pointer_cast<Component>(c))
                {
                    components.erase(c);
                    return true;
                }
            }
            return false;
        }

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
        std::shared_ptr<std::vector<std::shared_ptr<T>>> GetComponents() const
        {
            std::shared_ptr<std::vector<std::shared_ptr<T>>> _components;

            for (const auto& c : components)
            {
                if (std::dynamic_pointer_cast<T>(c))
                {
                    _components->push_back(c);
                }
            }
            return _components;
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

        template <class T>
        std::shared_ptr<std::vector<std::weak_ptr<T>>> GetComponentsWeak() const
        {
            std::shared_ptr<std::vector<std::weak_ptr<T>>> _components;

            for (const auto& c : components)
            {
                if (std::dynamic_pointer_cast<T>(c))
                {
                    _components->push_back(c);
                }
            }
            return _components;
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

            child->DestroyAllChildrenComponents();

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
                child->DestroyAllChildrenComponents();
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
        void components_start();

        void components_update();

        void components_lateUpdate();

        void components_draw() const;

        void components_call_collisionstay(std::shared_ptr<GameObject>& object);
    };  // namespace ComponentEngine
}  // namespace ComponentEngine
