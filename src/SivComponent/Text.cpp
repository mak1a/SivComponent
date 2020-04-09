
#define NO_S3D_USING
#include "Text.hpp"

namespace ComponentEngine::Siv
{
    void Text::Draw() const
    {
        // const auto pos = GetGameObject().lock()->transform().GetWorldPosition();

        const auto t = s3d::Transformer2D(GetGameObject().lock()->transform().GetMatrix(), true);

        if (drawAt)
        {
            if (shadow.enable)
            {
                font(text).drawAt(shadow.offset, shadow.color);
            }

            font(text).drawAt({0, 0}, color);
        }
        else
        {
            if (shadow.enable)
            {
                font(text).draw(shadow.offset, shadow.color);
            }

            font(text).draw(s3d::Vec2(0, 0), color);
        }
    }
}  // namespace ComponentEngine::Siv
