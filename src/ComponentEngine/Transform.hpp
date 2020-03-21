#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include <string>
#include "../Utilities/Utilities.hpp"

namespace ComponentEngine
{
    class GameObject;
    struct SivTransform2D
    {
    private:
        s3d::Vec2 position;
        // radian
        double rotate;
        double scale;

        mutable s3d::Mat3x2 matrix;

        //描画順にのみ影響するz軸情報
        // int z;

    private:
        friend GameObject;

        void update_matrix(const s3d::Mat3x2& _mat) const
        {
            matrix.setProduct(_mat, CreateMatrix());
        }

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

        SivTransform2D& SetWorldPosition(const s3d::Vec2& _position)
        {
            position = matrix.inversed().transform(_position);
            return *this;
        }

        s3d::Vec2 GetWorldPosition() const
        {
            return matrix.transform(position);
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

        SivTransform2D& SetScale(double _scale)
        {
            scale = _scale;
            return *this;
        }

        double GetScale() const noexcept
        {
            return scale;
        }

        // SivTransform2D& SetZ(int _z)
        // {
        //     z = _z;
        //     return *this;
        // }

        // int GetZ() const noexcept
        // {
        //     return z;
        // }

    public:
        SivTransform2D()
            : SivTransform2D({0, 0}, 0.0, 1.0)
        {
        }

        SivTransform2D(const s3d::Vec2& _pos, double _rotate, double _scale)
            : position(_pos)
            , rotate(_rotate)
            , scale(_scale)
        {
        }

    private:
    public:
        [[nodiscard]] s3d::Mat3x2 CreateMatrix() const
        {
            return s3d::Mat3x2::Translate(position).rotated(rotate).scaled(scale);
            // return std::make_unique<s3d::Transformer2D>(s3d::Mat3x2::Translate(position).rotated(rotate).scaled(scale), true);
        }

        [[nodiscard]] s3d::Mat3x2 CreateInversedMatrix() const
        {
            return s3d::Mat3x2::Translate(position).rotated(rotate).scaled(scale).inversed();
            // return std::make_unique<s3d::Transformer2D>(s3d::Mat3x2::Translate(position).rotated(rotate).scaled(scale), true);
        }
    };
}  // namespace ComponentEngine
