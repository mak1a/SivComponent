
#pragma once

#define NO_S3D_USING
//#include <iostream>
#include "../ComponentEngine/ComponentEngine.hpp"

#include "LoadBalancing-cpp/inc/Client.h"
#include "NetworkSystem.hpp"

namespace ComponentEngine::Photon
{
    class PhotonInitScene : public ComponentEngine::IScene
    {
        void Setup() override;
    };
}  // namespace PhotonComponent
