
#pragma once

#define NO_USING_S3D
#include <Siv3D.hpp>
#include "../ComponentEngine/ComponentEngine.hpp"

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

        struct
        {
            bool enable = false;
            s3d::Color color = s3d::Palette::Gray;
            s3d::Vec2 offset = {10, 10};
        } shadow;

    public:
        Text()
            : color(s3d::Palette::White)
            , text(U"text")
            , font(25)
            , drawAt(true)
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

        Text& SetShadowEnable(bool enable)
        {
            shadow.enable = enable;
            return *this;
        }

        bool GetShadowEnable() const
        {
            return shadow.enable;
        }

        Text& SetShadowColor(s3d::Color color)
        {
            shadow.color = color;
            return *this;
        }

        s3d::Color GetShadowColor() const
        {
            return shadow.color;
        }

        Text& SetShadowOffset(s3d::Vec2 offset)
        {
            shadow.offset = offset;
            return *this;
        }

        s3d::Vec2 GetShadowOffset() const
        {
            return shadow.offset;
        }

    private:
        void Draw() const override;
    };
}  // namespace ComponentEngine::Siv
