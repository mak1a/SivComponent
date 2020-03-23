
#pragma once

#define NO_USING_S3D
#include <Siv3D.hpp>
#include "ComponentEngine.hpp"

namespace ComponentEngine::Siv
{
    class Text : public ComponentEngine::AttachableComponent
    {
    private:
        using String = s3d::String;

        s3d::ColorF color;
        s3d::String text;
        s3d::Font font;
        bool drawAt;

    public:
        Text()
            : color(s3d::Palette::White)
            , text(U"text")
            , font(30)
            , drawAt(false)
        {
        }

        Text& SetText(const String& _text)
        {
            text = _text;
            return *this;
        }

        String GetText() const noexcept
        {
            return text;
        }

        Text& SetColor(const s3d::ColorF& _color)
        {
            color = _color;
            return *this;
        }

        s3d::ColorF GetColor() const noexcept
        {
            return color;
        }

        Text& SetFont(const s3d::Font& _font)
        {
            font = _font;
            return *this;
        }

        s3d::Font GetFont() const noexcept
        {
            return font;
        }

        Text& SetDrawAt(bool _drawAt)
        {
            drawAt = _drawAt;
            return *this;
        }

        bool GetDrawAt() const noexcept
        {
            return drawAt;
        }

    private:
        void Draw() const override
        {
            const auto pos = GetGameObject().lock()->transform().GetWorldPosition();

            if (drawAt)
            {
                font(text).drawAt(pos, color);
            }
            else
            {
                font(text).draw(pos, color);
            }
        }
    };
}  // namespace ComponentEngine::Siv
