
#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include "ComponentEngine.hpp"

namespace ComponentEngine::Siv
{
    class Line : public ComponentEngine::AttachableComponent
    {
    private:
        double length;
        double thickness;
        s3d::ColorF color;

    public:
        Line& SetLength(double _length)
        {
            length = _length;
            return *this;
        }

        double GetLength() const noexcept
        {
            return length;
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
            s3d::Line(0, 0, length, 0).draw(thickness, color);
        }
    };

    class Triangle : public ComponentEngine::AttachableComponent
    {
    public:
        s3d::Triangle triangle;
        s3d::ColorF color;

    protected:
        void Draw() const override
        {
            triangle.draw(color);
        }
    };

    class Circle : public ComponentEngine::AttachableComponent
    {
    private:
        s3d::ColorF color;
        double r;

    public:
        Circle& SetColor(const s3d::ColorF& _color)
        {
            color = _color;
            return *this;
        }

        s3d::ColorF GetColor() const noexcept
        {
            return color;
        }

        Circle& SetR(double _r)
        {
            r = _r;
            return *this;
        }

        double GetR() const noexcept
        {
            return r;
        }

        Circle()
            : color(s3d::Palette::White)
            , r(50)
        {
        }

    protected:
        void Draw() const override
        {
            s3d::Circle(0, 0, r).draw(color);
        }
    };

    class Rect : public ComponentEngine::AttachableComponent
    {
    public:
        s3d::Vec2 Size;
        s3d::ColorF color;

        Rect()
            : Size(100, 100)
            , color(s3d::Palette::White)
        {
        }

    protected:
        void Draw() const override
        {
            s3d::RectF(0, 0, Size).draw(color);
        }
    };
}  // namespace ComponentEngine::Siv
