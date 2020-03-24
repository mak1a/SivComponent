
#include "InstantiateEntry.hpp"
#include "../SivComponent/SivComponent.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Player/PlayerBullet.hpp"

void InstantiateEntry_Game(ComponentEngine::SceneCommon& common)
{
    common.AddObjectCreator("Player", [&]() {
        //        auto obj = GameObject::Create();
        auto obj = std::make_shared<GameObject>();
        obj->AddComponent<Player>();
        obj->AddComponent<Siv::Circle>()->SetShape({0, 0, 10});
        return obj;
    });

    common.AddObjectCreator("PlayerBullet", [&]() {
        auto obj = GameObject::Create();
        obj->AddComponent<Siv::Circle>()->SetShape({0, 0, 8}).SetColor(s3d::Palette::Darkblue);
        auto bu = obj->AddComponent<Bullet>();
        bu->lifetime = 0.8;
        bu->attack = 10;
        //仮設定
        return obj;
    });
}
