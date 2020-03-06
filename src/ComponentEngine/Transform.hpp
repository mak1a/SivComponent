#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include <string>
#include "../Utilities/Utilities.hpp"

namespace ComponentEngine
{
    struct SivTransform2D
    {
    public:
        s3d::Vec2 pos;
        // radian
        double rotate;
        s3d::Vec2 scale;
        //描画順にのみ影響するz軸
        int z;

    public:
        SivTransform2D& SetRotateByAngle(double angle)
        {
            rotate = Utilities::DegToRad(angle);
            return *this;
        }

    public:
        std::string name;

        SivTransform2D()
            : SivTransform2D({0, 0}, 0.0, {1, 1})
        {
        }

        SivTransform2D(const s3d::Vec2& _pos, double _rotate, const s3d::Vec2& _scale)
            : pos(_pos)
            , rotate(_rotate)
            , scale(_scale)
        {
        }

    private:
    public:
        auto PushTransform() const -> decltype(std::make_unique<s3d::Transformer2D>())
        {
            return std::make_unique<s3d::Transformer2D>(s3d::Mat3x2::Translate(pos).rotated(rotate).scaled(scale));
            // return std::move(trans);
        }

        void PopTransform() const {}
    };
}  // namespace ComponentEngine
