
#include "Enemy.hpp"
#include "../Bullet.hpp"
#include "EnemyManager.hpp"

// void Enemy::Start() {}

//距離60 * 4.5　より　距離250以下になったらコアを狙うようにする

void Enemy::Update()
{
    fireInterval -= s3d::Scene::DeltaTime();

    if (fireInterval < 0)
    {
        fireInterval = 0.3;
        enemyManager->CreateBullet(*this, {0, 1}, 60, 4.5);
    }
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
