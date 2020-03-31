
#include "PlayerCore.hpp"

void PlayerCore::OnStayCollision(std::shared_ptr<GameObject>& other)
{
    if (other->GetTag() == UserDef::Tag::EnemyBullet)
    {
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