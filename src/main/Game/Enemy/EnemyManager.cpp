#include "EnemyManager.hpp"
#include "Enemy.hpp"

int EnemyManager::GenerateEnemyNumber()
{
    return enemynumber++;
}

void EnemyManager::Start2()
{
    enemynumber = networkSystem->GetPlayerNumberInRoom() * 100000;

    CreateStandardEnemy({-400, -400});
    CreateStandardEnemy({400, -400});
    CreateStandardEnemy({400, 400});
    CreateStandardEnemy({-400, 400});
}

void EnemyManager::CreateStandardEnemy(const s3d::Vec2& position)
{
    std::shared_ptr<GameObject> obj = GameObject::Create();
    obj->SetName("standard enemy");
    constexpr s3d::Rect shape(-10, -10, 20, 20);

    auto enemy = obj->AddComponent<Enemy>();
    obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
    obj->AddComponent<Collision::RectCollider>()->SetShape(shape);
    obj->AddComponent<Siv::Rect>()->SetShape(shape).SetColor(s3d::Palette::Darkred);

    GetGameObject().lock()->AddChild(obj)->SetPosition(position);

    const int num = GenerateEnemyNumber();
    enemy->SetEnemyNumber(num);
}

void EnemyManager::Update() {}
