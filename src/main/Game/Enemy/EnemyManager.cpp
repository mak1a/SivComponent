#include "EnemyManager.hpp"
#include "../../CustomEventList.hpp"
#include "../Bullet.hpp"
#include "Enemy.hpp"

void EnemyManager::Start2()
{
    for (int i = 0; i < 3; ++i)
    {
        CreateStandardEnemy(nullptr);
    }

    bullets = GetGameObject().lock()->CreateChild();
    bullets->SetName("bullets");
}

void EnemyManager::CreateStandardEnemy(ExitGames::Common::Dictionary<nByte, int>* dic)
{
    std::shared_ptr<GameObject> obj = GetGameObject().lock()->CreateChild();
    obj->SetName("standard enemy");
    constexpr s3d::Rect shape(-10, -10, 20, 20);

    auto enemy = obj->AddComponent<Enemy>();
    enemy->enemyManager = this;
    enemy->TargetObject = playercore;

    obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
    obj->AddComponent<Collision::RectCollider>()->SetShape(shape);
    obj->AddComponent<Siv::Rect>()->SetShape(shape).SetColor(s3d::Palette::Darkred);

    if (dic == nullptr)
    {
        auto dic = enemy->CreateAndGetData();
        networkSystem->GetClient().opRaiseEvent(true, *dic, CustomEvent::EnemyGenerate);
    }
    else
    {
        enemy->SetDataFromDictionary(*dic);
    }
}

void EnemyManager::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::EnemyGenerate)
    {
        return;
    }

    auto dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int> >(eventContent).getDataCopy();

    // typeだけ取る
    const int type = *dic.getValue(DataName::Enemy::Type);

    //辞書データを丸投げ
    if (type == EnemyType::Standard)
    {
        CreateStandardEnemy(&dic);
    }
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
