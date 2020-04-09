#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class ReturnTitleBt : public Photon::AttachableComponentPhotonCallbacks
{
    void Start2();
    void OnClick();
};