
#include "InstantiateEntry.hpp"
#include "../SivComponent/SivComponent.hpp"

#include "Game/Player/Player.hpp"
#include "Game/Player/PlayerBullet.hpp"

#include "Game/Enemy/Enemy.hpp"
#include "Game/Field/MapWall.hpp"

void InstantiateEntry_Game(ComponentEngine::SceneCommon& common)
{
    //プレイヤー
    common.AddObjectCreator("Player", []() {
        //        auto obj = GameObject::Create();
        auto obj = std::make_shared<GameObject>();
        obj->AddComponent<Player>();
        // obj->AddComponent<Siv::Circle>()->SetShape({0, 0, 10});
        constexpr s3d::RectF shape(-10, -10, 20, 20);
        obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Player);
        obj->AddComponent<Collision::RectCollider>()->SetShape(shape);
        obj->AddComponent<Siv::Rect>()->SetShape(shape);
        return obj;
    });

    //プレイヤーの弾
    common.AddObjectCreator("PlayerBullet", []() {
        auto obj = GameObject::Create();
        constexpr s3d::Circle shape(0, 0, 6);

        obj->SetTag(UserDef::Tag::PlayerBullet);
        obj->AddComponent<Siv::Circle>()->SetShape(shape).SetColor(s3d::Palette::Blue);
        auto bu = obj->AddComponent<Bullet>();
        obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::PlayerBullet);
        obj->AddComponent<Collision::CircleCollider>()->SetShape(shape);
        //数値は仮設定
        bu->lifetime = 1.5;
        bu->attack = 10;
        return obj;
    });

    //マップを囲う壁
    common.AddObjectCreator("MapFieldWall", []() {
        std::shared_ptr<GameObject> obj = GameObject::Create();

        obj->AddComponent<MapWall>();
        obj->SetTag(UserDef::Tag::Wall);
        obj->SetName("wall");
        constexpr s3d::RectF rect(-300, -(1000 + 600 * 2) / 2, 600, 1000 + 600 * 2);

        obj->AddComponent<Siv::Rect>()->SetShape(rect).SetColor(s3d::Palette::Gray);
        obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Field);
        obj->AddComponent<Collision::RectCollider>()->SetShape(rect);

        return obj;
    });

    //敵
    common.AddObjectCreator("EnemyStandard", []() {
        std::shared_ptr<GameObject> obj = GameObject::Create();
        constexpr s3d::Rect shape(-10, -10, 20, 20);

        obj->AddComponent<Enemy>();
        obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
        obj->AddComponent<Collision::RectCollider>()->SetShape(shape);
        obj->AddComponent<Siv::Rect>()->SetShape(shape).SetColor(s3d::Palette::Darkred);
        return obj;
    });

    common.AddObjectCreator("EnemyBomb", []() {
        std::shared_ptr<GameObject> obj = GameObject::Create();

        return obj;
    });

    common.AddObjectCreator("EnemyKiller", []() {
        std::shared_ptr<GameObject> obj = GameObject::Create();
        return obj;
    });
}
