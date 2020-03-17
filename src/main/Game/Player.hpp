#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>
#include "../../Utilities/IntervalCall.hpp"
#include "PhotonComponent.hpp"

using namespace ComponentEngine;

class Player : public Photon::AttachableComponentPhotonCallbacks
{
    void Start2() override;
    void Update() override;

    bool isMine;
    // void Start2() override;

    Utilities::IntervalCall syncpos;

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
    void SyncPos();

    void PrintCall()
    {
        s3d::Print(U"call");
    }

    Player()
        : syncpos((int32_t)1000, [&]() { PrintCall(); })
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
