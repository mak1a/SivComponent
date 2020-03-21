#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>

// #include <list>

// #include "../ComponentEngine/AttachableComponent.hpp"
// #include "../ComponentEngine/GameObject.hpp"
// #include "../ComponentEngine/IComponent.hpp"
// #include "../UserDefinition/CollisionLayer.hpp"

namespace ComponentEngine::Collision
{
    class CollisionObject;
    class ColliderBase
    {
        friend CollisionObject;

    protected:
        std::variant<s3d::Circle, s3d::Rect, s3d::RectF, s3d::Line, s3d::Triangle, s3d::Quad, s3d::RoundRect, s3d::Polygon, s3d::LineString> shape;

    public:
        // template <class OtherShape>
        // bool intersects(const OtherShape& other) const
        // {
        //     return std::visit([&](const auto& s) { return s.intersects(other); }, shape);
        // }

        bool intersects(const ColliderBase& other) const
        {
            return std::visit([&](const auto& s) { return s.intersects(other); }, shape);
        }
    };
}  // namespace ComponentEngine::Collision
