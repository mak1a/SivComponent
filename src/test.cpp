#include "test.hpp"
#include "PhotonComponent/PhotonComponent.hpp"

class PrintServerTime : public Photon::AttachableComponentPhotonCallbacks
{
    bool printflag = true;
    int cnt = -1;
    int before;

    void Start2() override
    {
        networkSystem->Connect();
    }

    void Update() override
    {
        if (s3d::KeySpace.down())
        {
            printflag = !printflag;
            cnt = -1;
        }

        if (cnt > 0)
        {
            cnt--;
        }

        if (cnt == 0)
        {
            printflag = false;
            return;
        }

        if (!printflag)
        {
            return;
        }

        if (networkSystem->GetServerTime() < before)
        {
            s3d::Print(U"------------------------stop");
            cnt = 3;
        }
        before = networkSystem->GetServerTime();

        s3d::Print(networkSystem->GetServerTime());
    }
};

void TestScene::Setup()
{
    CreateAndGetObject()->AddComponent<PrintServerTime>();

    // mode1
    // matrix check

    auto x = CreateAndGetObject();
    x->SetPosition({300, 200});
    x->AddComponent<MoveLeft>();
    // x->transform().SetRotateByAngle(45);
    x->SetScale(2);

    const auto r = s3d::Rect(0, 0, 40, 60);
    x->AddComponent<Siv::Rect>()->SetShape(r);

    s3d::Array<s3d::Vec2> v = {{100, 0}, {0, 100}, {-100, 0}, {0, -100}};

    for (const auto& pos : v)
    {
        auto y = CreateAndGetObject();
        x->AddChild(y);

        y->SetPosition(pos);

        y->AddComponent<Siv::Rect>()->SetShape(r);
    }

    // mode2
    // collision and cursor check

    // auto cir = CreateAndGetObject();
    // cir->AddComponent<ComponentEngine::Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
    // // cir->AddComponent<StayPrint>();
    // const auto c = s3d::Circle(300, 200, 50);
    // cir->AddComponent<ComponentEngine::Collision::CircleCollider>()->SetShape(c);
    // cir->AddComponent<Siv::Circle>()->SetShape(c);

    // cir = CreateAndGetObject();
    // cir->AddComponent<ComponentEngine::Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
    // // cir->AddComponent<StayPrint>();
    // cir->AddComponent<ComponentEngine::Collision::CircleCollider>()->SetShape(c);
    // cir->AddComponent<Siv::Circle>()->SetShape(c).SetColor(s3d::Palette::Green);
    // auto up = CreateAndGetObject();
    // up->SetScale(2);
    // up->AddChild(cir);

    // auto rec = CreateAndGetObject();

    // rec->AddComponent<ComponentEngine::Collision::CollisionObject>(UserDef::CollisionLayer::Enemy);
    // const auto r = s3d::Rect(100, 100, 40, 60);
    // rec->AddComponent<ComponentEngine::Collision::RectCollider>()->SetShape(r);
    // rec->AddComponent<Siv::Rect>()->SetShape(r);

    // auto mousemake = [&](const s3d::Color& color) {
    //     auto mouse = CreateAndGetObject();
    //     mouse->AddComponent<ComponentEngine::Collision::CollisionObject>(UserDef::CollisionLayer::Player);
    //     mouse->AddComponent<Siv::MouseChase>();
    //     mouse->AddComponent<Siv::Circle>()->SetShape({0, 0, 15}).SetColor(color);
    //     mouse->AddComponent<Collision::CircleCollider>()->SetShape({0, 0, 15});
    //     mouse->AddComponent<StayPrint>();
    //     return mouse;
    // };

    // auto dummy3 = CreateAndGetObject();
    // dummy3->SetScale(2);
    // dummy3->AddChild(mousemake(s3d::Palette::Yellow));

    // auto dummy = CreateAndGetObject();
    // dummy->SetPosition({100, 100}).SetScale(1.2);
    // dummy->AddChild(mousemake(s3d::Palette::Lightcoral));

    // auto dummy2 = CreateAndGetObject();
    // dummy2->SetRotateByAngle(45);
    // dummy2->AddChild(mousemake(s3d::Palette::Lightblue));

    // auto moveobj = CreateAndGetObject();
    // moveobj->SetScale(2).SetRotateByAngle(30);
    // moveobj->AddChild(rec);
}