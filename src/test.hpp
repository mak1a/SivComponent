
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

class MoveLeft : public ComponentEngine::AttachableComponent
{
    void Update() override
    {
        auto g = GetGameObject().lock();
        auto p = g->GetLocalPosition();
        g->SetLocalPosition(p.moveBy(1, 0));
    }
};

class TestScene : public ComponentEngine::IScene
{
    void Setup() override;
};

#endif /* test_hpp */
