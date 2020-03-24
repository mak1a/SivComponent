#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "Player.hpp"

class BulletManager : public AttachableComponent
{
    ComponentEngine::SceneCommon::Functype inst;

    void Start() override;

    void CreateBullet();

    void Update() override;

public:
    std::shared_ptr<Player> player;
};

class Bullet : public AttachableComponent
{
public:
    int attack;
    double lifetime;

    void Update() override;

    void SetMove(const s3d::Vec2& target, double speed);

private:
    s3d::Vec2 moveValue;
};
