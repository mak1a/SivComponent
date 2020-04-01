#pragma once

#include "../../../PhotonComponent/PhotonComponent.hpp"
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
    double spd;
    int life = 100;

    //線形補間用
    s3d::Vec2 targetPos;
    s3d::Vec2 movehistory;

    Utilities::IntervalCall syncpos;
    std::shared_ptr<GameObject> camera;

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
