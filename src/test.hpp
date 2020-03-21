
#ifndef test_hpp
#define test_hpp

#include "ComponentEngine/ComponentEngine.hpp"

using namespace ComponentEngine;

class StayPrint : public ComponentEngine::AttachableComponent
{
    void Start() override
    {
        s3d::Print(U"Start");
    }

    void Update() override
    {
        s3d::Print(U"");
    }

    void OnStayCollision(std::shared_ptr<GameObject>& object) override
    {
        s3d::Print(U"Hit!");
    }
};

class TestScene : public ComponentEngine::IScene
{
    void Setup() override
    {
        auto cir = CreateAndGetObject();
        cir->AddComponent<ComponentEngine::Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
        // cir->AddComponent<StayPrint>();
        const auto c = s3d::Circle(300, 200, 50);
        cir->AddComponent<ComponentEngine::Collision::CircleCollider>()->SetShape(c);
        cir->AddComponent<Siv::Circle>()->SetShape(c);

        auto rec = CreateAndGetObject();

        rec->AddComponent<ComponentEngine::Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
        const auto r = s3d::Rect(100, 100, 40, 60);
        rec->AddComponent<ComponentEngine::Collision::RectCollider>()->SetShape(r);
        rec->AddComponent<Siv::Rect>()->SetShape(r);

        auto mouse = CreateAndGetObject();
        mouse->AddComponent<ComponentEngine::Collision::CollisionObject>(UserDef::CollisionLayer::Player);
        mouse->AddComponent<Siv::MouseChase>();
        mouse->AddComponent<Siv::Circle>()->SetShape({0, 0, 15}).SetColor(s3d::Palette::Lightcoral);
        mouse->AddComponent<Collision::CircleCollider>()->SetShape({0, 0, 15});
        mouse->AddComponent<StayPrint>();
    }
};

#endif /* test_hpp */
