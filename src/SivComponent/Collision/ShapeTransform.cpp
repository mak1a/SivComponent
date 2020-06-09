
#include "ShapeTransform.hpp"

namespace ComponentEngine::Collision
{
    s3d::Quad transformed(const s3d::RectF& rect, const s3d::Mat3x2 mat)
    {
        const auto tl = mat.transform(rect.tl());
        const auto tr = mat.transform(rect.tr());
        const auto br = mat.transform(rect.br());
        const auto bl = mat.transform(rect.bl());
        return s3d::Quad(tl, tr, br, bl);
    }

    // s3d::RoundRect transformed(const s3d::RoundRect& rrect, const s3d::Mat3x2 mat)
    // {
    //     const auto tl = mat.transform(s3d::Vec2{rrect.x, rrect.y});
    //     const auto tr = mat.transform(rrect.tr());
    //     const auto br = mat.transform(rrect.br());
    //     const auto bl = mat.transform(rrect.bl());

    //     return s3d::Quad(tl, tr, br, bl);
    // }

    s3d::Quad transformed(const s3d::Quad& quad, const s3d::Mat3x2 mat)
    {
        const auto tl = mat.transform(quad.p0);
        const auto tr = mat.transform(quad.p1);
        const auto br = mat.transform(quad.p2);
        const auto bl = mat.transform(quad.p3);
        return s3d::Quad(tl, tr, br, bl);
    }

    s3d::Circle transformed(const s3d::Circle& circle, const s3d::Mat3x2 mat)
    {
        const auto center = mat.transform(circle.center);

        const auto a = center.distanceFrom(mat.transform(circle.center.movedBy(circle.r, 0)));

        return s3d::Circle(center, a);
    }

    s3d::Line transformed(const s3d::Line& line, const s3d::Mat3x2 mat)
    {
        return s3d::Line(mat.transform(line.begin), mat.transform(line.end));
    }

    s3d::Triangle transformed(const s3d::Triangle& triangle, const s3d::Mat3x2 mat)
    {
        return s3d::Triangle(mat.transform(triangle.p0), mat.transform(triangle.p1), mat.transform(triangle.p2));
    }
}  // namespace ComponentEngine::Collision
