#pragma once

#define NO_USING_S3D
#include "SivComponent/SivComponent.hpp"

struct TitleScene : public ComponentEngine::IScene
{
    void Setup() override;
};
