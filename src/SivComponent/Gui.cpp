
#define NO_S3D_USING
#include "Gui.hpp"

namespace ComponentEngine::Siv
{
    void RadioButton::Draw() const
    {
        const auto t = s3d::Transformer2D(GetGameObject().lock()->transform().GetMatrix(), true);

        if (drawAt)
        {
            changed = s3d::SimpleGUI::RadioButtonsAt(index, elements, s3d::Vec2::Zero());
        }
        else
        {
            changed = s3d::SimpleGUI::RadioButtons(index, elements, s3d::Vec2::Zero());
        }
    }
}  // namespace ComponentEngine::Siv