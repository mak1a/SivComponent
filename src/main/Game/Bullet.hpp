#pragma once

#include "../../ComponentEngine/ComponentEngine.hpp"
//#include "Player/Player.hpp"

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
