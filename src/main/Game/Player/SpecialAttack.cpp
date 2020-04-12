
#include "SpecialAttack.hpp"
#include "../Enemy/EnemyManager.hpp"
#include "Player.hpp"

void ISpecialAttack::Start2()
{
    playerAnime = GetGameObject().lock()->GetComponent<PlayerAnimation>();

    Start3();
}

void ISpecialAttack::Special()
{
    if (!hassp)
    {
        return;
    }
    hassp = false;
    playerAnime->OnBomb();
    OnSpecial();
}

void SpyBomb::Start3()
{
    //??????
    eManager = GetGameObject().lock()->GetScene().lock()->FindObject("EnemyManager")->GetComponent<EnemyManager>();
}

void SpyBomb::OnSpecial()
{
    s3d::Print(eManager->DestroyAllBullets());
}
