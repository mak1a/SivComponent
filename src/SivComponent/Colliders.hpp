#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>

#include <list>

#include "../ComponentEngine/AttachableComponent.hpp"
#include "../ComponentEngine/IComponent.hpp"
#include "../UserDefinition/CollisionLayer.hpp"

namespace ComponentEngine::Collision
{
    class ColliderBase
    {
    protected:
        std::variant<s3d::Circle, s3d::Rect, s3d::RectF, s3d::Line, s3d::Triangle, s3d::Quad, s3d::RoundRect, s3d::Polygon, s3d::LineString> shape;

    public:
        template <class OtherShape>
        bool intersects(const OtherShape& other) const
        {
            return std::visit([&](const auto& s) { return s.intersects(other); }, shape);
        }

        void draw(const s3d::ColorF& color = s3d::Palette::White) const
        {
            std::visit([=](const auto& s) { s.draw(color); }, shape);
        }
    };

    template <class Shape>
    class Collider : public ColliderBase
    {
    public:
        void SetShape(const Shape& _shape)
        {
            shape = _shape;
        }

        void GetShape() const
        {
            return shape;
        }

        bool intersects(const Collider& other) const
        {
            return intersects(other.shape);
        }
    };

    using CircleCollider = Collider<s3d::Circle>;
    using RectCollider = Collider<s3d::Rect>;
    using RectFCollider = Collider<s3d::RectF>;
    using LineCollider = Collider<s3d::Line>;
    using TriangleCollider = Collider<s3d::Triangle>;
    using QuadCollider = Collider<s3d::Quad>;
    using RoundRectCollider = Collider<s3d::RoundRect>;
    using PolygonCollider = Collider<s3d::Polygon>;
    using LineStringCollider = Collider<s3d::LineString>;

    using Layer = unsigned char;

    //衝突判定を取りたい物体に一つだけつける
    class CollisionObject : public AttachableComponent
    {
    private:
        const Layer layer;

    public:
        CollisionObject(UserDef::CollisionLayer _layer)
            : layer(static_cast<Layer>(_layer))
        {
        }

        Layer GetLayer() const noexcept
        {
            return layer;
        }
    };

    class CollisionSystem
    {
    private:
        std::list<ColliderBase*> colliders;

    public:
    };

    // template <class Shape>
    // class ColliderBase : public IComponent  //, public ICollider
    // {
    // private:
    //     Shape shape;

    // public:
    //     virtual bool intersects(const s3d::Circle& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }
    //     virtual bool intersects(const s3d::Rect& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }
    //     virtual bool intersects(const s3d::RectF& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }
    //     virtual bool intersects(const s3d::Line& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }
    //     virtual bool intersects(const s3d::Triangle& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }
    //     virtual bool intersects(const s3d::Quad& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }
    //     virtual bool intersects(const s3d::RoundRect& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }
    //     virtual bool intersects(const s3d::Shape2D& collider) const override
    //     {
    //         return collider.asPolygon().intersects(shape);
    //     }
    //     virtual bool intersects(const s3d::Polygon& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }
    //     virtual bool intersects(const s3d::LineString& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }

    //     virtual bool intersects(const ICollider& collider) const override
    //     {
    //         return collider.intersects(shape);
    //     }
    // };
}  // namespace ComponentEngine::Collision
