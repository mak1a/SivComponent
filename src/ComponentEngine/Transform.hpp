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
        s3d::Mat3x2 matrix;

        //描画順にのみ影響するz軸情報
        // int z;

    private:
        friend GameObject;

        // get set
    private:
        SivTransform2D& SetPosition(const s3d::Vec2& _position, const s3d::Mat3x2& parentMatrix)
        {
            position = _position;
            update_matrix(parentMatrix);
            return *this;
        }

    public:
        s3d::Vec2 GetPosition() const noexcept
        {
            return position;
        }

    private:
        SivTransform2D& SetWorldPosition(const s3d::Vec2& _position, const s3d::Mat3x2& inversedMatrix, const s3d::Mat3x2& parentMatrix)
        {
            position = inversedMatrix.transform(_position);
            update_matrix(parentMatrix);
            return *this;
        }

    public:
        s3d::Vec2 GetWorldPosition() const
        {
            return matrix.transform(s3d::Vec2::Zero());
        }

    private:
        SivTransform2D& SetRotateByRadian(double _rotate, const s3d::Mat3x2& parentMatrix)
        {
            rotate = _rotate;
            update_matrix(parentMatrix);
            return *this;
        }

        SivTransform2D& SetRotateByAngle(double angle, const s3d::Mat3x2& parentMatrix)
        {
            rotate = Utilities::DegToRad(angle);
            update_matrix(parentMatrix);
            return *this;
        }

    public:
        double GetRotate() const noexcept
        {
            return rotate;
        }

    private:
        SivTransform2D& SetScale(double _scale, const s3d::Mat3x2& parentMatrix)
        {
            scale = _scale;
            update_matrix(parentMatrix);
            return *this;
        }

    public:
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
            matrix = s3d::Mat3x2::Identity();
            // matrixはparent設定したときに変更できるのでok
        }

    private:
        void update_matrix(const s3d::Mat3x2& parentMatrix)
        {
            auto after = parentMatrix.transform(position).moveBy(-parentMatrix._31, -parentMatrix._32);

            //自分の変換座標による行列を生成
            const auto currentmatrix = create_matrix(after, scale, rotate, parentMatrix.transform(position));

            //親の行列と掛け合わせる
            matrix.setProduct(parentMatrix, currentmatrix);
        }

    public:
        //変換操作を統一させるための関数
        static s3d::Mat3x2 create_matrix(const s3d::Vec2 trans, double scale, double rotate, const s3d::Vec2 rotatecenter = {0, 0})
        {
            return s3d::Mat3x2::Scale(scale).translated(trans).rotated(rotate, rotatecenter);
        }

        // [[nodiscard]] s3d::Mat3x2 CreateMatrix() const
        // {
        //     return create_matrix(position, scale, rotate, GetWorldPosition());
        // }

        // [[nodiscard]] s3d::Mat3x2 CreateInversedMatrix() const
        // {
        //     return CreateMatrix().inversed();
        // }

        [[nodiscard]] const s3d::Mat3x2 GetMatrix() const
        {
            return matrix;
        }
    };
}  // namespace ComponentEngine
