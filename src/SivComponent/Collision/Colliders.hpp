#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>

#include "ColliderBase.hpp"

// #include <list>

#include "../../ComponentEngine/AttachableComponent.hpp"
// #include "../ComponentEngine/GameObject.hpp"
// #include "../ComponentEngine/IComponent.hpp"
#include "CollisionSystem.hpp"

namespace ComponentEngine::Collision
{
    template <class Shape>
    class Collider : public AttachableComponent, public ColliderBase
    {
        virtual void Start() override final
        {
            GetGameObject().lock()->template GetComponent<CollisionSystem>()->Register(this);
        }

        virtual void OnDestroy() override final
        {
            GetGameObject().lock()->template GetComponent<CollisionSystem>()->Dispose(this);
        }

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
}  // namespace ComponentEngine::Collision
