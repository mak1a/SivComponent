#pragma once

#include "../../ComponentEngine/IScene.hpp"
#include "../../PhotonComponent/PhotonComponent.hpp"

class PlayerCustom : public ComponentEngine::IScene
{
    void Setup() override;
};
