﻿
#include "InstantiateEntry.hpp"
#include "../SivComponent/SivComponent.hpp"
#include "Game/Bullet.hpp"
#include "Game/Enemy/Enemy.hpp"
#include "Game/Field/MapWall.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Player/PlayerInfoGUI.hpp"

void InstantiateEntry_Game(ComponentEngine::SceneCommon& common)
{
    //プレイヤー
    common.AddObjectCreator("Player", []() {
        //        auto obj = GameObject::Create();
        auto obj = std::make_shared<GameObject>();
        const auto player = obj->AddComponent<Player>();
        constexpr double size = 19;
        constexpr s3d::RectF shape(-size / 2, -size / 2, size, size);
        constexpr auto r = size * 0.38;
        constexpr s3d::Circle cir(0, 0, r);
        obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Player);
        obj->AddComponent<Collision::CircleCollider>()->SetShape(cir.stretched(-2));

        //枠
        obj->AddComponent<Siv::Rect>()->SetShape(shape);

        //周りの円
        obj->AddComponent<Siv::Circle>()->SetShape(cir).SetColor(s3d::Palette::Black);

        //中心の円
        (player->centerCircle = obj->AddComponent<Siv::Circle>())->SetShape(cir.stretched(-0.5));

        //ボムフレーム
        obj->AddComponent<Siv::RectFrame>()->SetShape(shape).SetColor(s3d::Palette::Blue).SetInnerThickness(1).SetOuterThickness(0);
        obj->AddComponent<PlayerAnimation>();

        //ライフ表示
        auto txtobj = obj->CreateChild();
        txtobj->SetPosition({0, size * 0.9});
        const auto text = txtobj->AddComponent<Siv::Text>();
        text->SetFont(s3d::Font(14, s3d::Typeface::Medium)).SetColor(s3d::Palette::Black).SetDrawAt(true);
        auto view = txtobj->AddComponent<PlayerLifeView>();
        view->player = player;
        view->text = text;
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
        bu->lifetime = 1.2;
        bu->attack = 10;
        return obj;
    });

    //敵の弾
    // common.AddObjectCreator("EnemyBullet", []() {
    //     auto obj = GameObject::Create();
    //     constexpr s3d::Circle shape(0, 0, 6);

    //     obj->SetTag(UserDef::Tag::EnemyBullet);
    //     obj->AddComponent<Siv::Circle>()->SetShape(shape).SetColor(s3d::Palette::Red);
    //     auto bu = obj->AddComponent<Bullet>();
    //     obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::EnemyBullet);
    //     obj->AddComponent<Collision::CircleCollider>()->SetShape(shape);
    //     //数値は仮設定
    //     bu->lifetime = 1.5;
    //     bu->attack = 10;
    //     return obj;
    // });

    //マップを囲う壁
    common.AddObjectCreator("MapFieldWall", []() {
        std::shared_ptr<GameObject> obj = GameObject::Create();

        obj->AddComponent<MapWall>();
        obj->SetTag(UserDef::Tag::Wall);
        obj->SetName("wall");
        constexpr int width = 2300, height = 600;
        constexpr s3d::RectF rect(-width / 2, -height / 2, width, height);

        obj->AddComponent<Siv::Rect>()->SetShape(rect).SetColor(s3d::Palette::Gray);
        obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Field);
        obj->AddComponent<Collision::RectCollider>()->SetShape(rect);

        return obj;
    });

    //プレイヤーUI
    common.AddObjectCreator("PlayerUIDisplay", []() {
        std::shared_ptr<GameObject> obj = GameObject::Create();
        auto listview = obj->AddComponent<PlayerListView>();

        const s3d::Rect shape(0, 0, 120, 80), barshape(0, 0, 100, 8);

        auto back = obj->AddComponent<ComponentEngine::Siv::Rect>();
        back->SetShape(shape).SetColor(s3d::Color(40, 40, 40, 200));

        auto name = obj->CreateChild()->SetPosition({5, 2}).SetName("PlayerName").AddComponent<ComponentEngine::Siv::Text>();
        name->SetFont(s3d::Font(15, s3d::Typeface::Bold)).SetText(U"プレイヤー999").SetColor(s3d::Color(255, 255, 255)).SetDrawAt(false);

        obj->CreateChild()
            ->SetPosition({70, 25})
            .SetName("SPText")
            .AddComponent<ComponentEngine::Siv::Text>()
            ->SetFont(s3d::Font(13, s3d::Typeface::Black))
            .SetText(U"SP:")
            .SetColor(s3d::Color(220, 220, 220))
            .SetDrawAt(false);

        auto spicon = obj->CreateChild()->SetPosition({100, 25}).SetName("SPIcon").SetRotateByAngle(45).AddComponent<Siv::Rect>();
        spicon->SetColor(s3d::Palette::Orange).SetShape({0, 0, 10, 10});

        constexpr s3d::Vec2 barpos(10, 52);

        obj->CreateChild()->SetPosition(barpos).SetName("WhiteBar").AddComponent<ComponentEngine::Siv::Rect>()->SetShape(barshape.stretched(3, 3)).SetColor(s3d::Palette::White);

        obj->CreateChild()->SetPosition(barpos).SetName("RedBar").AddComponent<ComponentEngine::Siv::Rect>()->SetShape(barshape).SetColor(s3d::Palette::Red);

        auto green = obj->CreateChild()->SetPosition(barpos).SetName("GreenBar").AddComponent<ComponentEngine::Siv::Rect>();
        green->SetShape(barshape).SetColor(s3d::Palette::Green);

        listview->background = back;
        listview->greenbar = green;
        listview->playername = name;
        listview->spicon = spicon;

        return obj;
    });

    // //敵
    // common.AddObjectCreator("EnemyStandard", []() {
    //     std::shared_ptr<GameObject> obj = GameObject::Create();
    //     constexpr s3d::Rect shape(-10, -10, 20, 20);

    //     obj->AddComponent<Enemy>();
    //     obj->AddComponent<Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
    //     obj->AddComponent<Collision::RectCollider>()->SetShape(shape);
    //     obj->AddComponent<Siv::Rect>()->SetShape(shape).SetColor(s3d::Palette::Darkred);
    //     return obj;
    // });

    // common.AddObjectCreator("EnemyBomb", []() {
    //     std::shared_ptr<GameObject> obj = GameObject::Create();

    //     return obj;
    // });

    // common.AddObjectCreator("EnemyKiller", []() {
    //     std::shared_ptr<GameObject> obj = GameObject::Create();
    //     return obj;
    // });
}
