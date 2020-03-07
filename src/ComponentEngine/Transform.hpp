#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include <string>
#include "../Utilities/Utilities.hpp"

namespace ComponentEngine
{
    struct SivTransform2D
    {
    private:
        s3d::Vec2 position;
        // radian
        double rotate;
        s3d::Vec2 scale;
        //描画順にのみ影響するz軸情報
        int z;
        std::string name;

    public:
        SivTransform2D& SetPosition(const s3d::Vec2& _position)
        {
            position = _position;
            return *this;
        }

        s3d::Vec2 GetPosition() const noexcept
        {
            return position;
        }

        SivTransform2D& SetRotateByRadian(double _rotate)
        {
            rotate = _rotate;
            return *this;
        }

        SivTransform2D& SetRotateByAngle(double angle)
        {
            rotate = Utilities::DegToRad(angle);
            return *this;
        }

        double GetRotate() const noexcept
        {
            return rotate;
        }

        SivTransform2D& SetScale(const s3d::Vec2& _scale)
        {
            scale = _scale;
            return *this;
        }

        s3d::Vec2 GetScale() const noexcept
        {
            return scale;
        }

        SivTransform2D& SetZ(int _z)
        {
            z = _z;
            return *this;
        }

        int GetZ() const noexcept
        {
            return z;
        }

        SivTransform2D& SetName(const std::string& _name)
        {
            name = _name;
            return *this;
        }

        std::string GetName() const noexcept
        {
            return name;
        }

    public:
        SivTransform2D()
            : SivTransform2D({0, 0}, 0.0, {1, 1})
        {
        }

        SivTransform2D(const s3d::Vec2& _pos, double _rotate, const s3d::Vec2& _scale)
            : position(_pos)
            , rotate(_rotate)
            , scale(_scale)
        {
        }

    private:
    public:
        auto PushTransform() const -> decltype(std::make_unique<s3d::Transformer2D>())
        {
            return std::make_unique<s3d::Transformer2D>(s3d::Mat3x2::Translate(position).rotated(rotate).scaled(scale));
            // return std::move(trans);
        }

        void PopTransform() const {}
    };
}  // namespace ComponentEngine
