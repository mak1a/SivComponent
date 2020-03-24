
#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include "Collision/ShapeTransform.hpp"
#include "ComponentEngine.hpp"

namespace ComponentEngine::Siv
{
    class Line : public ComponentEngine::AttachableComponent
    {
        using Shape = s3d::Line;

    private:
        Shape shape;

        double thickness;
        s3d::ColorF color;

    public:
        Line& SetShape(const Shape& _shape)
        {
            shape = _shape;
            return *this;
        }

        Shape GetShape() const noexcept
        {
            return shape;
        }

        Line& SetThickness(double _thickness)
        {
            thickness = _thickness;
            return *this;
        }

        double GetThickness() const noexcept
        {
            return thickness;
        }

        Line& SetColor(const s3d::ColorF& _color)
        {
            color = _color;
            return *this;
        }

        s3d::ColorF GetColor() const noexcept
        {
            return color;
        }

    protected:
        void Draw() const override
        {
            Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()).draw(thickness, color);
        }
    };

    class Triangle : public ComponentEngine::AttachableComponent
    {
        using Shape = s3d::Triangle;

    private:
        s3d::Triangle shape;
        s3d::ColorF color;

    public:
        Triangle& SetShape(const Shape& _shape)
        {
            shape = _shape;
            return *this;
        }

        Shape GetShape() const noexcept
        {
            return shape;
        }

        Triangle& SetColor(const s3d::ColorF& _color)
        {
            color = _color;
            return *this;
        }

        s3d::ColorF GetColor() const noexcept
        {
            return color;
        }

        Triangle()
            : shape(100)
        {
        }

    protected:
        void Draw() const override
        {
            Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()).draw(color);
        }
    };

    class Circle : public ComponentEngine::AttachableComponent
    {
        using Shape = s3d::Circle;

    private:
        Shape shape;
        s3d::ColorF color;

    public:
        Circle& SetShape(const Shape& _shape)
        {
            shape = _shape;
            return *this;
        }

        Shape GetShape() const noexcept
        {
            return shape;
        }

        Circle& SetColor(const s3d::ColorF& _color)
        {
            color = _color;
            return *this;
        }

        s3d::ColorF GetColor() const noexcept
        {
            return color;
        }

        Circle()
            : color(s3d::Palette::White)
            , shape(0, 0, 100)
        {
        }

    protected:
        void Draw() const override
        {
            Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix()).draw(color);
        }
    };

    class Rect : public ComponentEngine::AttachableComponent
    {
        using Shape = s3d::RectF;
        using ThisType = Rect;

    private:
        Shape shape;
        s3d::ColorF color;

    public:
        ThisType& SetShape(const Shape& _shape)
        {
            shape = _shape;
            return *this;
        }

        Shape GetShape() const noexcept
        {
            return shape;
        }

        ThisType& SetColor(const s3d::ColorF& _color)
        {
            color = _color;
            return *this;
        }

        s3d::ColorF GetColor() const noexcept
        {
            return color;
        }

        Rect()
            : color(s3d::Palette::White)
            , shape(0, 0, 100, 100)
        {
        }

    protected:
        void Draw() const override
        {
            auto s = Collision::transformed(shape, GetGameObject().lock()->transform().GetMatrix());
            s.draw(color);
        }
    };
}  // namespace ComponentEngine::Siv
