
#include "InstantiateEntry.hpp"
#include "../SivComponent/SivComponent.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Player/PlayerBullet.hpp"

void InstantiateEntry_Game(ComponentEngine::SceneCommon& common)
{
    //プレイヤー
    common.AddObjectCreator("Player", [&]() {
        //        auto obj = GameObject::Create();
        auto obj = std::make_shared<GameObject>();
        obj->AddComponent<Player>();
        // obj->AddComponent<Siv::Circle>()->SetShape({0, 0, 10});
        obj->AddComponent<Siv::Rect>()->SetShape(s3d::Rect(0, 0, 20, 20));
        return obj;
    });

    //プレイヤーの弾
    common.AddObjectCreator("PlayerBullet", [&]() {
        auto obj = GameObject::Create();
        s3d::Circle shape(0, 0, 6);
        obj->AddComponent<Siv::Circle>()->SetShape(shape).SetColor(s3d::Palette::Blue);
        auto bu = obj->AddComponent<Bullet>();
        bu->lifetime = 1.5;
        bu->attack = 10;
        //仮設定
        return obj;
    });
}
