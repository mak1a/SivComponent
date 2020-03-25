#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include <boost/noncopyable.hpp>

#include <list>
#include <memory>

#include "../UserDefinition/ObjectTag.hpp"
#include "IComponent.hpp"
#include "Transform.hpp"

namespace ComponentEngine
{
    using Transform = SivTransform2D;

    class IScene;

    class GameObject final : public std::enable_shared_from_this<GameObject>, private boost::noncopyable
    {
        friend class IScene;
        friend class CollisionSystem;

    public:
        [[nodiscard]] std::weak_ptr<GameObject>& GetParent();
        [[nodiscard]] std::list<std::shared_ptr<GameObject>>& GetChildren();

        bool IsInitializedAll();

        GameObject& SetPosition(const s3d::Vec2& _position);
        s3d::Vec2 GetPosition() const;
        GameObject& SetWorldPosition(const s3d::Vec2& _position);
        s3d::Vec2 GetWorldPosition() const;
        GameObject& SetLocalPosition(const s3d::Vec2& _position);
        s3d::Vec2 GetLocalPosition() const;
        GameObject& SetRotateByRadian(double _rotate);
        GameObject& SetRotateByAngle(double angle);
        double GetRotate() const noexcept;
        GameObject& SetScale(double _scale);
        double GetScale() const noexcept;

        s3d::Vec2 LookAt(const std::shared_ptr<GameObject>& target) const;
        s3d::Vec2 LookAt(const s3d::Vec2& targetWorldPos) const;

        GameObject& SetName(const std::string& _name);
        std::string GetName() const noexcept;
        GameObject& SetTag(UserDef::Tag _tag);
        UserDef::Tag GetTag() const;

        std::weak_ptr<IScene> GetScene();

        GameObject& SetActive(bool _active);
        bool GetActive() const noexcept;
        Transform& transform();

        void AddChild(const std::shared_ptr<GameObject>& child, bool pushfront = false);
        [[nodiscard]] std::shared_ptr<GameObject> CreateAndGetChild();
        void SetParent(const std::weak_ptr<GameObject>& newParent, bool pushfront = false);
        std::shared_ptr<GameObject> FindChild(const std::shared_ptr<GameObject>& child) const;
        std::shared_ptr<GameObject> FindChild(const std::string& name) const;

        void DeleteChild(const std::shared_ptr<GameObject>& child);
        //デストラクタに任せると伝播方向の関係でポインタが無効になって死ぬので先に呼べ
        //すべての子オブジェクトを破壊する
        void DestroyAll();

        GameObject();
        explicit GameObject(const Transform& trans);

        ~GameObject() = default;

        // IScene API
        void components_start();

        void components_update();

        void components_lateUpdate();

        void components_draw() const;

        // IScene event API
        void components_call_collisionstay(std::shared_ptr<GameObject>& object);

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

    private:
        // weak_ptrを使えるようにするためにshared_ptrにしたが、正直生ポインタでいいところではある。
        //メモリリークのリスクの方が大きい気もする。
        std::list<std::shared_ptr<IComponent>> components;

        //親子オブジェクト
        std::weak_ptr<GameObject> parent;
        std::list<std::shared_ptr<GameObject>> children;

        // Start更新済みかどうか
        bool initializedAll = false;

        // GameObjectは必ずtransformを持つ
        Transform _transform;
        // 上位オブジェクトへの参照
        std::weak_ptr<IScene> scene;

        std::string name = "unnamed";
        UserDef::Tag objecttag;

        bool active;

        static s3d::Mat3x2 identity_matrix;
        s3d::Mat3x2& parent_matrix() const;

        //子オブジェクトに変換行列を適用する
        void update_children_matrix();
        auto FindChildItr(const std::shared_ptr<GameObject>& child) const -> decltype(std::find(children.begin(), children.end(), child));

        auto FindChildItr(const std::string& name) const -> decltype(std::find(children.begin(), children.end(), children.front()));

        //子オブジェクトを削除せずに外します
        bool RemoveChild(const std::shared_ptr<GameObject>& child);

        void DestroyAllChildren();

        void DestroyAllChildrenComponents();
    };  // namespace ComponentEngine
}  // namespace ComponentEngine
