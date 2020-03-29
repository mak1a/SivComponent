#pragma once

#include <list>

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

    void OnWall();
    void OnEnemy();
    void OnEnemyBullet();

    void OnStayCollision(std::shared_ptr<GameObject>& other) override;

    bool isMine;
    //線形補間用
    s3d::Vec2 targetPos;
    s3d::Vec2 movehistory;

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
    //プレイヤーの位置初期化用配列
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
    PlayerMaster();
    std::vector<std::shared_ptr<Player>> players;
};
