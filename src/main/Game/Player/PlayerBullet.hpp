#pragma once

#include "ComponentEngine.hpp"
#include "Player.hpp"

class BulletManager : public AttachableComponent
{
    ComponentEngine::SceneCommon::Functype inst;

    void Start() override
    {
        inst = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetInstantiate("PlayerBullet");
    }

    void CreateBullet()
    {
        auto playerpos = player->GetGameObject().lock()->transform().GetWorldPosition();

        auto bullet = inst();
        GetGameObject().lock()->AddChild(bullet);

        bullet->transform().SetPosition(player->GetGameObject().lock()->transform().GetPosition());
    }

    void Update() override
    {
        if (s3d::MouseL.down())
        {
            CreateBullet();
        }
    }

public:
    std::shared_ptr<Player> player;
};

class Bullet : public AttachableComponent
{
public:
    int attack;
    double lifetime;

    void SetMove(s3d::Vec2 targetPos, double speed)
    {
        // auto currentPos = GetGameObject().lock()->GetLocalPosition();
    }

private:
    s3d::Vec2 moveValue;
};