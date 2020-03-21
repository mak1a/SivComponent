#define NO_S3D_USING
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "PhotonComponent/PhotonComponent.hpp"

#include "Matching.hpp"
#include "PhotonComponent/PhotonInitScene.hpp"
#include "Title.hpp"
#include "main/Game/Game.hpp"
#include "main/InstantiateEntry.hpp"

//#include <unicode/unistr.h>

using std::cout;
using std::endl;

class StayPrint : public ComponentEngine::AttachableComponent
{
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

void Main()
{
    ComponentEngine::SceneManager manager;

    manager.RegisterScene<TestScene>("test");
    manager.RegisterScene<ComponentEngine::Photon::PhotonInitScene>("PhotonInitOnceLoad");
    manager.RegisterScene<TitleScene>("Title");
    manager.RegisterScene<Matching>("Matching");
    manager.RegisterScene<Game>("Game");

    InstantiateEntry_Game(manager.GetCommon());

    while (s3d::System::Update())
    {
        manager.UpdateCurrentScene();
    }

    manager.End();
}
