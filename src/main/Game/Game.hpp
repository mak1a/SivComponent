
#pragma once

#include "ComponentEngine.hpp"
#include "SivComponent.hpp"
#include "PhotonComponent.hpp"

class Game : public ComponentEngine::IScene
{
    void Setup() override
    {
        s3d::Print(U"Game");
    }
};
