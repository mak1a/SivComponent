﻿

#include "Bullet.hpp"

void Bullet::Update()
{
    auto obj = GetGameObject().lock();
    auto pos = obj->GetPosition();
    obj->SetPosition(pos.moveBy(moveValue * s3d::Scene::DeltaTime()));

    lifetime -= s3d::Scene::DeltaTime();
    if (lifetime < 0)
    {
        auto obj = GetGameObject().lock();
        obj->GetScene().lock()->Destroy(obj);
    }
}


void Bullet::SetMove(const s3d::Vec2& target, double speed)
{
    moveValue = target.normalized() * speed;
    // moveValue = GetGameObject().lock()->LookAt(target) * speed;
}

void Bullet::OnEnterCollision(std::shared_ptr<GameObject>& other) {
    s3d::ClearPrint();
    s3d::Print << U"Enter";
}

void Bullet::OnStayCollision(std::shared_ptr<GameObject>& other)
{
    s3d::Print << U"Stay";
    //何かに当たったら消える
    /*auto obj = GetGameObject().lock();
    obj->GetScene().lock()->Destroy(obj);*/
}

void Bullet::OnExitCollision(std::shared_ptr<GameObject>& other) {
    s3d::Print << U"Exit";
}
