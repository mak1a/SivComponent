#include "EnemyManager.hpp"
#include "Enemy.hpp"

void EnemyManager::Start2()
{
    auto creator = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetInstantiate("EnemyStandard");
    auto obj = GetGameObject().lock();

    obj->AddChild(creator())->SetPosition({0, 0});
    obj->AddChild(creator())->SetPosition({600, 0});
    obj->AddChild(creator())->SetPosition({0, 600});
    obj->AddChild(creator())->SetPosition({600, 600});
}