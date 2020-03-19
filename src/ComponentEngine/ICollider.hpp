#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>

namespace ComponentEngine::Collider
{
    class ICollider
    {
    public:
        virtual bool intersects(const s3d::Circle&) const = 0;
        virtual bool intersects(const s3d::Rect&) const = 0;
        virtual bool intersects(const s3d::RectF&) const = 0;
        virtual bool intersects(const s3d::Line&) const = 0;
        virtual bool intersects(const s3d::Triangle&) const = 0;
        virtual bool intersects(const s3d::Quad&) const = 0;
        // virtual bool intersects(const s3d::Ellipse&) const = 0;
        virtual bool intersects(const s3d::RoundRect&) const = 0;
        virtual bool intersects(const s3d::Shape2D&) const = 0;
        virtual bool intersects(const s3d::Polygon&) const = 0;
        virtual bool intersects(const s3d::LineString&) const = 0;

        virtual bool intersects(const ICollider&) const = 0;
        virtual ~ICollider() = 0;
    };
}  // namespace ComponentEngine::Collider