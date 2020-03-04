
#pragma once

#include <Siv3D.hpp>
#include <string>

namespace ComponentEngine
{
    struct SivTransform
    {
    public:
        Vec2 pos;
        double rotate;
        Vec2 scale;

    public:
        std::string name;

        SivTransform()
            : SivTransform({0, 0}, 0.0, {1, 1})
        {
        }

        SivTransform(const Vec2& _pos, double _rotate, const Vec2& _scale)
            : pos(_pos)
            , rotate(_rotate)
            , scale(_scale)
        {
        }

    private:
    public:
        Transformer2D&& PushTransform()
        {
            return Transformer2D(s3d::Mat3x2::Translate(pos).Rotate(rotate).Scale(scale));
        }

        void PopTransform() {}
    };
}  // namespace ComponentEngine
