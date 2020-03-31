
#include "Enemy.hpp"
#include "../Bullet.hpp"

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

        // if (life <= 0)
        // {
        //     GetGameObject().lock()->GetScene().lock()->Destroy(this->GetGameObject().lock());
        // }
    }
}
