

#include "PlayerBullet.hpp"

void BulletManager::Start()
{
    inst = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetInstantiate("PlayerBullet");
}

void BulletManager::CreateBullet()
{
    auto playerpos = player->GetGameObject().lock()->GetWorldPosition();

    auto bullet = inst();
    GetGameObject().lock()->AddChild(bullet);

    bullet->SetPosition(player->GetGameObject().lock()->GetPosition());

    bullet->GetComponent<Bullet>()->SetMove(s3d::Cursor::PosF(), 190);
}

void BulletManager::Update()
{
    if (s3d::MouseL.down())
    {
        CreateBullet();
    }
}

//----

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
    moveValue = GetGameObject().lock()->LookAt(target) * speed;
}