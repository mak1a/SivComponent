#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>

#include "ICollider.hpp"

// #include <list>

#include "../../ComponentEngine/AttachableComponent.hpp"
// #include "../ComponentEngine/GameObject.hpp"
// #include "../ComponentEngine/IComponent.hpp"
#include "CollisionSystem.hpp"

namespace ComponentEngine::Collision
{
    template <class Shape>
    class Collider : public ICollider, public AttachableComponent
    {
        Shape shape;

    public:
        Shape GetShape() const
        {
            return shape;
        }

        void SetShape(const Shape& _shape)
        {
            shape = _shape;
        }

        Shape& row_shape()
        {
            return shape;
        }

        virtual bool intersects(const s3d::Circle& other) const override
        {
            return other.intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

        virtual bool intersects(const s3d::Rect& other) const override
        {
            return other.intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

        virtual bool intersects(const s3d::RectF& other) const override
        {
            return other.intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

        virtual bool intersects(const s3d::Line& other) const override
        {
            return other.intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

        virtual bool intersects(const s3d::Triangle& other) const override
        {
            return other.intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

        virtual bool intersects(const s3d::Quad& other) const override
        {
            return other.intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

        virtual bool intersects(const s3d::RoundRect& other) const override
        {
            return other.intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

        virtual bool intersects(const s3d::Polygon& other) const override
        {
            return other.intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

        virtual bool intersects(const s3d::LineString& other) const override
        {
            return other.intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

        //        virtual bool intersects(const ICollider& other) const override
        virtual bool intersects(ICollider* other) const override
        {
            return other->intersects(Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()));
        }

    private:
        virtual void Start() override final
        {
            GetGameObject().lock()->template GetComponent<CollisionObject>()->Register(this);
        }

        virtual void OnDestroy() override final
        {
            GetGameObject().lock()->template GetComponent<CollisionObject>()->Dispose(this);
        }
    };

    using CircleCollider = Collider<s3d::Circle>;
    // using RectCollider = Collider<s3d::Rect>;
    // using RectFCollider = Collider<s3d::RectF>;
    using RectCollider = Collider<s3d::RectF>;
    using LineCollider = Collider<s3d::Line>;
    // using TriangleCollider = Collider<s3d::Triangle>;
    using QuadCollider = Collider<s3d::Quad>;
    // using RoundRectCollider = Collider<s3d::RoundRect>;
    // using PolygonCollider = Collider<s3d::Polygon>;
    // using LineStringCollider = Collider<s3d::LineString>;

    // template <class Shape>
    // class Collider : public AttachableComponent, public ColliderBase
    // {
    //     virtual void Start() override final
    //     {
    //         GetGameObject().lock()->template GetComponent<CollisionSystem>()->Register(this);
    //     }

    //     virtual void OnDestory() override final
    //     {
    //         GetGameObject().lock()->template GetComponent<CollisionSystem>()->Dispose(this);
    //     }

    // public:
    //     void SetShape(const Shape& _shape)
    //     {
    //         shape = _shape;
    //     }

    //     void GetShape() const
    //     {
    //         return shape;
    //     }

    //     bool intersects(const Collider& other) const
    //     {
    //         return intersects(other.shape);
    //     }
    // };

    // using CircleCollider = Collider<s3d::Circle>;
    // using RectCollider = Collider<s3d::Rect>;
    // using RectFCollider = Collider<s3d::RectF>;
    // using LineCollider = Collider<s3d::Line>;
    // using TriangleCollider = Collider<s3d::Triangle>;
    // using QuadCollider = Collider<s3d::Quad>;
    // using RoundRectCollider = Collider<s3d::RoundRect>;
    // using PolygonCollider = Collider<s3d::Polygon>;
    // using LineStringCollider = Collider<s3d::LineString>;
}  // namespace ComponentEngine::Collision
