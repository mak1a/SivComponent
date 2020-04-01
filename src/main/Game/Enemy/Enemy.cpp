
#include "Enemy.hpp"
#include "../../../Utilities/Utilities.hpp"
#include "../Bullet.hpp"
#include "../Player/Player.hpp"
#include "../Player/PlayerManager.hpp"
#include "EnemyManager.hpp"

//速度60 * 生存時間4.5　より　距離250以下になったら優先的にコアを狙うようにする
void Enemy::SetTarget()
{
    const auto len = playerManager->players.size();

    //防衛オブジェクトを標的に設定
    fire.targetplayer = TargetObject;
    double distance = GetGameObject().lock()->GetPosition().distanceFrom(TargetObject->GetPosition());

    for (int i = 0; i < len; ++i)
    {
        const auto p = playerManager->players[i]->GetGameObject().lock();
        const auto d = GetGameObject().lock()->GetPosition().distanceFrom(p->GetPosition());

        //最短を更新
        if (d < distance)
        {
            fire.targetplayer = p;
            distance = d;
        }
    }

    //ターゲットプレイヤー番号を送信

    fire.rapidnum = 3;
    fire.reloadtime = 3.3;
}

void Enemy::Update()
{
    Move();
    Shot();
}

void Enemy::Shot()
{
    fire.interval -= s3d::Scene::DeltaTime();
    fire.reloadtime -= s3d::Scene::DeltaTime();

    //再ターゲットが可能になっていたら実行
    if (fire.reloadtime < 0)
    {
        SetTarget();
    }

    //連射回数が切れていたらなにもしない
    if (fire.rapidnum <= 0)
    {
        return;
    }

    if (fire.interval < 0)
    {
        fire.interval = 0.5;
        fire.rapidnum--;

        //攻撃ベクトルを計算
        auto target = fire.targetplayer->GetPosition();
        target = target - GetGameObject().lock()->GetPosition();

        enemyManager->CreateBullet(*this, target, 60, 4.5);
        enemyManager->CreateBullet(*this, target.rotated(Utilities::DegToRad(20)), 60, 4.5);
        enemyManager->CreateBullet(*this, target.rotated(Utilities::DegToRad(-20)), 60, 4.5);
    }
}

void Enemy::Move()
{
    auto diff = TargetObject->GetPosition() - GetGameObject().lock()->GetPosition();

    //接近の閾値以下なら移動はしない
    if (diff.length() < 150)
    {
        return;
    }
    auto g = GetGameObject().lock();
    const auto p = g->GetPosition() + diff.normalized() * spd * s3d::Scene::DeltaTime();
    g->SetPosition(p);
}

void Enemy::OnStayCollision(std::shared_ptr<GameObject>& obj)
{
    //弾当たったら
    if (obj->GetTag() == UserDef::Tag::PlayerBullet)
    {
        auto bullet = obj->GetComponent<Bullet>();

        //自分のプレイヤーが発射した弾でなければ何もしない
        if (!bullet->isMine)
        {
            return;
        }

        life -= bullet->attack;

        if (life <= 0)
        {
            // TODO: 消滅エフェクト
            GetGameObject().lock()->GetScene().lock()->Destroy(this->GetGameObject().lock());
        }
    }
}

EnemyManager* Enemy::enemyManager;
PlayerManager* Enemy::playerManager;