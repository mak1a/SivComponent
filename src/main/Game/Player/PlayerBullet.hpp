#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "Player.hpp"

class Bullet : public AttachableComponent
{
public:
    int attack;
    double lifetime;
    bool isMine;
    s3d::Vec2 moveValue;

    Bullet()
    {
        isMine = false;
    }

    void Update() override;

    void SetMove(const s3d::Vec2& target, double speed);
};

class BulletManager : public Photon::AttachableComponentPhotonCallbacks
{
    ComponentEngine::SceneCommon::Functype inst;
    double fireInterval;

    void Start2() override;

    void CreateBullet();

    void SendBulletInfo(std::shared_ptr<Bullet>& bullet);
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

    void Update() override;

public:
    std::shared_ptr<Player> player;
};
