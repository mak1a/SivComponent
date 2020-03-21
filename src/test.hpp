
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
        s3d::Print(U"update");
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
        cir->AddComponent<ComponentEngine::Collision::CollisionObject>(UserDef::CollisionLayer::Player);
        cir->AddComponent<StayPrint>();
        cir->AddComponent<ComponentEngine::Collision::CircleCollider>()->SetShape(s3d::Circle(0, 0, 100));

        auto rec = CreateAndGetObject();

        rec->AddComponent<ComponentEngine::Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
        rec->AddComponent<ComponentEngine::Collision::RectCollider>()->SetShape(s3d::Rect(-50, -50, 100, 100));
    }
};


#endif /* test_hpp */
