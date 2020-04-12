#include "EnemyManager.hpp"
#include "../../Common/CommonMemory.hpp"
#include "../../CustomEventList.hpp"
#include "../../Matching/Matching.hpp"
#include "../Bullet.hpp"
#include "Enemy.hpp"

constexpr double HPperSec[] = {4.0, 7.5, 15, 20, 24};

void EnemyManager::Start2()
{
    bullets = GetGameObject().lock()->CreateChild();
    bullets->SetName("Bullets");

    enemys = GetGameObject().lock()->CreateChild();
    enemys->SetName("Enemys");

    generator.persecond = HPperSec[CommonMemory::GetDifficulty()];
    // 4秒分の敵を最初に生成
    generator.generatableHP = generator.persecond * 4;
}

void EnemyManager::EnemyGenerate()
{
    while (0 < generator.generatableHP)
    {
        generator.generatableHP -= CreateStandardEnemy(nullptr);
    }
}

//敵の生成頻度をコントロール
void EnemyManager::Update()
{
    const int difficulty = CommonMemory::GetDifficulty();
    //    static double t = 1.0;
    // t += s3d::Random(-0.3, 0.3);
    // t = s3d::Clamp(t, 0.7, 10.0);
    generator.interval -= s3d::Scene::DeltaTime();
    generator.generatableHP += s3d::Scene::DeltaTime() * generator.persecond;

    //残り時間が短くなってきたら難易度をあげる
    if (timer->GetTime() < 40)
    {
        //生成間隔を半分に
        generator.interval -= s3d::Scene::DeltaTime();

        //生成可能数も増やす
        constexpr double d[] = {3.5, 4, 4, 7, 8};

        generator.generatableHP += s3d::Scene::DeltaTime() * d[difficulty];
    }

    if (generator.interval < 0)
    {
        EnemyGenerate();
        //次の生成まで1~5秒開ける
        generator.interval = s3d::Random(1.0, 7.0);
    }
}

int EnemyManager::CreateStandardEnemy(ExitGames::Common::Dictionary<nByte, int>* dic)
{
    std::shared_ptr<GameObject> obj = enemys->CreateChild();
    obj->SetName("standard enemy");
    constexpr s3d::Rect shape(-10, -10, 20, 20);

    auto enemy = obj->AddComponent<Enemy>();
    enemy->enemyManager = this;
    enemy->TargetObject = playercore;

    obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
    obj->AddComponent<Collision::RectCollider>()->SetShape(shape);
    obj->AddComponent<Siv::Rect>()->SetShape(shape).SetColor(s3d::Palette::Darkred);

    int retHP = 0;
    if (dic == nullptr)
    {
        auto d = enemy->GenerateAndGetData();
        retHP = enemy->GetLife();
        networkSystem->GetClient().opRaiseEvent(true, *d, CustomEvent::EnemyGenerate);
    }
    else
    {
        retHP = enemy->SetDataFromDictionary(*dic);
    }
    return retHP;
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
    auto obj = bullets->CreateChild();
    constexpr s3d::Circle shape(0, 0, 6);

    obj->SetTag(UserDef::Tag::EnemyBullet);
    obj->AddComponent<Siv::Circle>()->SetShape(shape).SetColor(s3d::Palette::Red);
    auto bu = obj->AddComponent<Bullet>();
    obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::EnemyBullet);
    obj->AddComponent<Collision::CircleCollider>()->SetShape(shape.stretched(-2));
    //数値は仮設定
    bu->lifetime = lifetime;
    bu->attack = attack;

    obj->SetPosition(enemy.GetGameObject().lock()->GetPosition());

    bu->SetMove(target, spd);
}

unsigned long EnemyManager::DestroyAllBullets()
{
    const auto& bu = bullets->GetChildren();

    const auto len = bu.size();

    auto scene = GetGameObject().lock()->GetScene().lock();

    for (auto& b : bu)
    {
        scene->Destroy(b);
    }

    return len;
}

void EnemyManager::AllEnemysDamage(int damage)
{
    for (auto& e : enemys->GetChildren())
    {
        e->GetComponent<Enemy>()->Damage(damage);
    }
}