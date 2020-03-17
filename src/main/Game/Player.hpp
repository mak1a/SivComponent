#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include "PhotonComponent.hpp"

using namespace ComponentEngine;

class Player : public Photon::AttachableComponentPhotonCallbacks
{
    bool isMine;
    // void Start2() override;

public:
    int playerNr;

    bool IsMine() const noexcept
    {
        return isMine;
    }

    void SetMine(bool ismine)
    {
        isMine = ismine;
    }

    void SendInstantiateMessage();

    Player()
    {
        isMine = false;
    }

public:
    static s3d::Vec2 playerInitpos[4];
};

namespace DataName::Player
{
    constexpr nByte playerNr = 1;
    constexpr nByte posX = 10;
    constexpr nByte posY = 11;
};  // namespace DataName::Player

class PlayerCreator : public Photon::AttachableComponentPhotonCallbacks
{
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;
};
