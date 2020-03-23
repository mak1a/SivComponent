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

        //現在までのオブジェクト関係から計算されたマトリックス(自分の数値も含む)
        mutable s3d::Mat3x2 matrix;

        //描画順にのみ影響するz軸情報
        // int z;

    private:
        friend GameObject;

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

        SivTransform2D& SetWorldPosition(const s3d::Vec2& _position, const s3d::Mat3x2& inversedMatrix)
        {
            position = inversedMatrix.transform(_position);
            s3d::Print(position);
            return *this;
        }

        s3d::Vec2 GetWorldPosition() const
        {
            return matrix.transform(s3d::Vec2::Zero());
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
        void update_matrix(const s3d::Mat3x2& _mat) const
        {
            auto movedpos = _mat.transform(position);
            matrix = create_matrix(movedpos, scale, rotate, movedpos);

            return;
            auto thismat = create_matrix(_mat.transform(position), scale, rotate, _mat.transform(position));
            // s3d::Mat3x2::Translate(position).rotated(rotate, _mat.transform(position)).scaled(scale)
            matrix.setProduct(_mat, thismat);
        }

    public:
        static s3d::Mat3x2 create_matrix(const s3d::Vec2 trans, double scale, double rotate, const s3d::Vec2 rotatecenter = {0, 0})
        {
            return s3d::Mat3x2::Scale(scale).rotated(rotate, rotatecenter).translated(trans);
        }

        [[nodiscard]] s3d::Mat3x2 CreateMatrix() const
        {
            //拡大させて回転させて移動
            return create_matrix(position, scale, rotate, GetWorldPosition());

            //移動させて回転させて拡大する
            return s3d::Mat3x2::Translate(position).rotated(rotate, GetWorldPosition()).scaled(scale);
        }

        [[nodiscard]] s3d::Mat3x2 CreateInversedMatrix() const
        {
            return CreateMatrix().inversed();
            // return std::make_unique<s3d::Transformer2D>(s3d::Mat3x2::Translate(position).rotated(rotate).scaled(scale), true);
        }

        [[nodiscard]] const s3d::Mat3x2 GetMatrix() const
        {
            return matrix;
        }
    };
}  // namespace ComponentEngine
