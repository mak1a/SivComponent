
#pragma once

#define NO_USING_S3D
#include <Siv3D.hpp>
#include "ComponentEngine.hpp"

namespace ComponentEngine::Siv
{
    class Line : public ComponentEngine::AttachableComponent
    {
    public:
        double length;
        double thickness;
        ColorF color;

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
    public:
        s3d::ColorF color;
        double r;

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
            RectF(0, 0, Size).draw(color);
        }
    };
}  // namespace ComponentEngine::Siv
