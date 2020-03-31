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

    void SetMove(const s3d::Vec2& target, double speed);

private:
    void Update() override;
    void OnStayCollision(std::shared_ptr<GameObject>& other) override;
};

class BulletManager : public Photon::AttachableComponentPhotonCallbacks
{
    ComponentEngine::SceneCommon::Functype inst;
    double fireInterval;

    void Start2() override;
    void Update() override;

    void CreateBullet();
    void SendBulletInfo(std::shared_ptr<Bullet>& bullet);
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

public:
    std::shared_ptr<Player> player;
};
