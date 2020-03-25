#pragma once

#include <list>

#define NO_S3D_USING
#include <Siv3D.hpp>
#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "../../../SivComponent/Collision/Colliders.hpp"
#include "../../../SivComponent/SivComponent.hpp"
#include "../../../Utilities/IntervalCall.hpp"

using namespace ComponentEngine;

class Player : public Photon::AttachableComponentPhotonCallbacks
{
    void Start2() override;
    void Update() override;
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

    bool isMine;
    s3d::Vec2 targetPos;

    // void Start2() override;

    Utilities::IntervalCall syncpos;

    double spd;

    std::shared_ptr<GameObject> master;

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

    Player();

private:
    void SyncPosWithEasing();

public:
    static s3d::Vec2 playerInitpos[4];
};

namespace DataName::Player
{
    constexpr nByte posX = 1;
    constexpr nByte posY = 2;
    constexpr nByte initalized = 4;
};  // namespace DataName::Player

//----------------------------------

class PlayerMaster : public Photon::AttachableComponentPhotonCallbacks
{
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;
    void leaveRoomEventAction(int playerNr, bool isInactive) override;
    void Start2() override;

    void Update() override;

    Utilities::IntervalCall initsync;

public:
    std::vector<std::shared_ptr<Player>> players;
};
