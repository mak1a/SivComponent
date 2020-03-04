
#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include <string>

namespace ComponentEngine
{
    struct SivTransform
    {
    public:
        s3d::Vec2 pos;
        double rotate;
        s3d::Vec2 scale;

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
        s3d::Transformer2D&& PushTransform()
        {
            return s3d::Transformer2D(s3d::Mat3x2::Translate(pos).Rotate(rotate).Scale(scale));
        }

        void PopTransform() {}
    };
}  // namespace ComponentEngine
