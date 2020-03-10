#pragma once

//#include"ComponentEngine.hpp"
#include "../ComponentEngine/AttachableComponent.hpp"
#include "../ComponentEngine/IScene.cpp"

namespace PhotonComponent
{
    class AttachableComponentPhotonCallbacks : public ComponentEngine::AttachableComponent
    {
        AttachableComponentPhotonCallbacks();
        virtual ~AttachableComponentPhotonCallbacks();
    };
}  // namespace PhotonComponent
