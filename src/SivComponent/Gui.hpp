
#pragma once

#define NO_USING_S3D
#include <Siv3D.hpp>
#include "ComponentEngine.hpp"

namespace ComponentEngine::Siv
{
    class TextBox : public ComponentEngine::AttachableComponent
    {
        mutable s3d::TextEditState textstate;
        int width = 500;
        int maxLength = 100;
        bool isEnabled = true;
        bool drawAt = true;

    public:
        TextBox& SetTextstate(const s3d::TextEditState& _textstate)
        {
            textstate = _textstate;
            return *this;
        }

        s3d::TextEditState GetTextstate() const noexcept
        {
            return textstate;
        }

        TextBox& SetWidth(int _width)
        {
            width = _width;
            return *this;
        }

        int GetWidth() const noexcept
        {
            return width;
        }

        TextBox& SetIsEnabled(bool _isEnabled)
        {
            isEnabled = _isEnabled;
            return *this;
        }

        bool GetIsEnabled() const noexcept
        {
            return isEnabled;
        }

        TextBox& SetIsDrawAt(bool _drawAt)
        {
            drawAt = _drawAt;
            return *this;
        }

        bool GetIsDrawAt() const noexcept
        {
            return drawAt;
        }

        TextBox& SetText(s3d::String _text)
        {
            textstate.text = _text;
            return *this;
        }

        TextBox& SetMaxLength(int _maxLength)
        {
            maxLength = _maxLength;
            return *this;
        }

        int GetMaxLength() const noexcept
        {
            return maxLength;
        }

        s3d::String GetText() const noexcept
        {
            return textstate.text;
        }

        bool IsActive() const
        {
            return textstate.active;
        }

    private:
        void Draw() const
        {
            if (drawAt)
            {
                s3d::SimpleGUI::TextBoxAt(textstate, {0, 0}, width, maxLength, isEnabled);
            }
            else
            {
                s3d::SimpleGUI::TextBox(textstate, {0, 0}, width, maxLength, isEnabled);
            }
        }
    };

    class Button : public ComponentEngine::AttachableComponent
    {
        // AttachableComponent*
    };
}  // namespace ComponentEngine::Siv
