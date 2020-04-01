
#include "PlayerCore.hpp"
#include "../Bullet.hpp"

void PlayerCore::OnStayCollision(std::shared_ptr<GameObject>& other)
{
    if (other->GetTag() == UserDef::Tag::EnemyBullet)
    {
        auto b = other->GetComponent<Bullet>();
        life -= b->attack;
        s3d::Print(life);
    }

    if (other->GetTag() == UserDef::Tag::PlayerBullet)
    {
        auto b = other->GetComponent<Bullet>();
        //フレンドリーファイア補正
        life -= (b->attack / 5);
        s3d::Print(life);
    }
}

void PlayerCore::SetLife(int _life)
{
    life = _life;
}

int PlayerCore::GetLife() const
{
    return life;
}