#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include <string>
#include "../Utilities/Utilities.hpp"

namespace ComponentEngine
{
    struct SivTransform
    {
    public:
        s3d::Vec2 pos;
        //弧度法
        double rotate;
        s3d::Vec2 scale;

    public:
        SivTransform& SetRotateByAngle(double angle)
        {
            rotate = Utilities::DegToRad(angle);
            return *this;
        }

    public:
        std::string name;

        SivTransform()
            : SivTransform({0, 0}, 0.0, {1, 1})
        {
        }

        SivTransform(const s3d::Vec2& _pos, double _rotate, const s3d::Vec2& _scale)
            : pos(_pos)
            , rotate(_rotate)
            , scale(_scale)
        {
        }

    private:
    public:
        auto PushTransform() -> decltype(std::make_unique<s3d::Transformer2D>())
        {
            return std::make_unique<s3d::Transformer2D>(s3d::Mat3x2::Translate(pos).rotated(rotate).scaled(scale));
            // return std::move(trans);
        }

        void PopTransform() {}
    };
}  // namespace ComponentEngine
