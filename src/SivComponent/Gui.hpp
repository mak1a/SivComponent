
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
        void Awake() override
        {
            width = 500;
            maxLength = 100;
            isEnabled = true;
            drawAt = true;
        }

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

        TextBox& SetDrawAt(bool _drawAt)
        {
            drawAt = _drawAt;
            return *this;
        }

        bool GetDrawAt() const noexcept
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
        void Draw() const override
        {
            if (drawAt)
            {
                s3d::SimpleGUI::TextBoxAt(textstate, s3d::Vec2(0, 0), width, maxLength, isEnabled);
            }
            else
            {
                s3d::SimpleGUI::TextBox(textstate, s3d::Vec2(0, 0), width, maxLength, isEnabled);
            }
        }
    };

    //実装をクソ雑にやってるので、デリゲートに登録するオブジェクトと生存期間合わせないと例外吐いて死亡
    class Button : public ComponentEngine::AttachableComponent
    {
        std::function<void()> delegate;
        unsigned width;
        bool isActive;
        s3d::String text;
        bool drawAt;

        void Draw() const override
        // void Update() override
        {
            bool pushed;

            if (drawAt)
            {
                pushed = s3d::SimpleGUI::ButtonAt(text, s3d::Vec2(0, 0), width, isActive);
            }
            else
            {
                pushed = s3d::SimpleGUI::Button(text, s3d::Vec2(0, 0), width, isActive);
            }

            if (pushed)
            {
                delegate();
            }
        }

        void Awake() override
        {
            width = 300;
            isActive = true;
            text = U"Button";
            drawAt = true;
            delegate = []() {};
        }

    public:
        Button& SetDelegate(const std::function<void()>& _delegate)
        {
            delegate = _delegate;
            return *this;
        }

        std::function<void()> GetDelegate() const noexcept
        {
            return delegate;
        }

        Button& SetWidth(unsigned _width)
        {
            width = _width;
            return *this;
        }

        unsigned GetWidth() const noexcept
        {
            return width;
        }

        Button& SetIsActive(bool _isActive)
        {
            isActive = _isActive;
            return *this;
        }

        bool GetIsActive() const noexcept
        {
            return isActive;
        }

        Button& SetText(const s3d::String& _text)
        {
            text = _text;
            return *this;
        }

        s3d::String GetText() const noexcept
        {
            return text;
        }

        Button& SetDrawAt(bool _drawAt)
        {
            drawAt = _drawAt;
            return *this;
        }

        bool GetDrawAt() const noexcept
        {
            return drawAt;
        }
    };
}  // namespace ComponentEngine::Siv
