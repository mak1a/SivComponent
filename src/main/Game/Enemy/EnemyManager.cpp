#include "EnemyManager.hpp"
#include "../Bullet.hpp"
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

    bullets = GetGameObject().lock()->CreateChild();
    bullets->SetName("bullets");
}

void EnemyManager::CreateStandardEnemy(const s3d::Vec2& position)
{
    std::shared_ptr<GameObject> obj = GameObject::Create();
    obj->SetName("standard enemy");
    constexpr s3d::Rect shape(-10, -10, 20, 20);

    auto enemy = obj->AddComponent<Enemy>();
    enemy->enemyManager = this;
    enemy->TargetObject = playercore;
    obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
    obj->AddComponent<Collision::RectCollider>()->SetShape(shape);
    obj->AddComponent<Siv::Rect>()->SetShape(shape).SetColor(s3d::Palette::Darkred);

    GetGameObject().lock()->AddChild(obj)->SetPosition(position);

    const int num = GenerateEnemyNumber();
    enemy->SetEnemyNumber(num);
}

void EnemyManager::CreateBullet(Enemy& enemy, const s3d::Vec2& target, double spd, double lifetime, int attack)
{
    auto obj = GetGameObject().lock()->CreateChild();
    constexpr s3d::Circle shape(0, 0, 6);

    obj->SetTag(UserDef::Tag::EnemyBullet);
    obj->AddComponent<Siv::Circle>()->SetShape(shape).SetColor(s3d::Palette::Red);
    auto bu = obj->AddComponent<Bullet>();
    obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::EnemyBullet);
    obj->AddComponent<Collision::CircleCollider>()->SetShape(shape);
    //数値は仮設定
    bu->lifetime = lifetime;
    bu->attack = attack;

    obj->SetPosition(enemy.GetGameObject().lock()->GetPosition());

    bu->SetMove(target, spd);
}

void EnemyManager::Update() {}
