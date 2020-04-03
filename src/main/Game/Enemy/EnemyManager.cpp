#include "EnemyManager.hpp"
#include "../../CustomEventList.hpp"
#include "../Bullet.hpp"
#include "Enemy.hpp"

namespace DataName
{
    constexpr nByte posX = 0;
    constexpr nByte posY = 1;
    constexpr nByte Number = 2;
    constexpr nByte Type = 9;
    constexpr nByte ServerTime = 99;
}  // namespace DataName

int EnemyManager::GenerateEnemyNumber()
{
    return enemynumber++;
}

void EnemyManager::Start2()
{
    enemynumber = networkSystem->GetPlayerNumberInRoom() * 100000;

    for (int i = 0; i < 3; ++i)
    {
        const double length = s3d::Random(600, 800);
        const double rad = s3d::Random(2 * s3d::Math::Pi);

        const s3d::Vec2 pos = s3d::Vec2(length, 0).rotate(rad);

        CreateStandardEnemy(pos, true);
    }

    bullets = GetGameObject().lock()->CreateChild();
    bullets->SetName("bullets");
}

void EnemyManager::CreateStandardEnemy(const s3d::Vec2& position, bool isMine, int enemyNumber)
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

    //番号が振られていなければユニークIDを生成
    //判定的にはisMine == false　でもいい
    int num = enemyNumber;
    if (enemyNumber == -1)
    {
        num = GenerateEnemyNumber();
    }
    enemy->SetEnemyNumber(num);
    enemy->SetIsMine(isMine);

    if (isMine)
    {
        //敵の生成を同期
        ExitGames::Common::Dictionary<nByte, int> dic;
        dic.put(DataName::posX, position.x);
        dic.put(DataName::posY, position.y);
        dic.put(DataName::Number, num);
        dic.put(DataName::Type, EnemyType::Standard);
        dic.put(DataName::ServerTime, networkSystem->GetServerTime());
        networkSystem->GetClient().opRaiseEvent(true, dic, CustomEvent::EnemyGenerate);
    }
}

void EnemyManager::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::EnemyGenerate)
    {
        return;
    }

    auto dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int> >(eventContent).getDataCopy();

    const int type = *dic.getValue(DataName::Type);

    if (type == EnemyType::Standard)
    {
        s3d::Vec2 pos;
        pos.x = *dic.getValue(DataName::posX);
        pos.y = *dic.getValue(DataName::posY);

        const int number = *dic.getValue(DataName::Number);
        const int time = *dic.getValue(DataName::ServerTime);

        // servertimeを元にposをずらす

        CreateStandardEnemy(pos, false, number);
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
