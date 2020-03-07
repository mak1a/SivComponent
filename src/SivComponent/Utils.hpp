#pragma once

#define NO_USING_S3D
#include <Siv3D.hpp>
#include "ComponentEngine.hpp"

namespace ComponentEngine::Siv
{
    class MouseChase : public ComponentEngine::AttachableComponent
    {
        void Start() override {}

        void Update() override
        {
            this->transform().SetPosition(Cursor::Pos());
        }
    };
}  // namespace ComponentEngine::Siv
