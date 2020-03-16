#pragma once

#include "PhotonComponent.hpp"

using namespace ComponentEngine;

class Player : public Photon::AttachableComponentPhotonCallbacks
{
    void Start() override;

    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;
};

namespace DataName::Player
{
    constexpr nByte playerNr = 1;
    constexpr nByte posX = 10;
    constexpr nByte posY = 11;
};  // namespace DataName::Player
