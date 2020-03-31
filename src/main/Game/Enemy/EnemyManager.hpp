#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class EnemyManager : public Photon::AttachableComponentPhotonCallbacks
{
    double randomValue;

    void Start2() override;
    void Update() override;
};