#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class EnemyManager : public Photon::AttachableComponentPhotonCallbacks
{
    void Start2() override;
    // void Update() override;
};