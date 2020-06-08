#include "test.hpp"
#include "PhotonComponent/PhotonComponent.hpp"

// class PrintServerTime : public Photon::AttachableComponentPhotonCallbacks
// {
//     bool printflag = true;
//     int cnt = -1;
//     int before;

//     void Start2() override
//     {
//         networkSystem->Connect();
//     }

//     void Update() override
//     {
//         if (s3d::KeySpace.down())
//         {
//             printflag = !printflag;
//             cnt = -1;
//         }

//         if (cnt > 0)
//         {
//             cnt--;
//         }

//         if (cnt == 0)
//         {
//             printflag = false;
//             return;
//         }

//         if (!printflag)
//         {
//             return;
//         }

//         if (networkSystem->GetServerTime() < before)
//         {
//             s3d::Print(U"------------------------stop");
//             cnt = 3;
//         }
//         before = networkSystem->GetServerTime();

//         s3d::Print(networkSystem->GetServerTime());
//     }
// };

class Hit : public AttachableComponent
{
    std::shared_ptr<Siv::Circle> circle;
    bool ishit;

    void Start()
    {
        circle = GetGameObject().lock()->GetComponent<Siv::Circle>();
    }

    void Update()
    {
        circle->SetColor(ishit ? s3d::Palette::Orange : s3d::Palette::White);

        ishit = false;
    }

    void OnStayCollision(std::shared_ptr<GameObject>& obj) override
    {
        ishit = true;
    }
};

void TestScene::Setup()
{
    // CreateAndGetObject()->AddComponent<PrintServerTime>();

    // mode1
    // matrix check

    const s3d::Rect rect(50, 100);

    auto a = CreateAndGetObject();
    a->SetPosition({50, 50});
    a->AddComponent<Collision::CollisionObject>();
    a->AddComponent<Collision::RectCollider>()->SetShape(rect);
    a->AddComponent<Siv::Rect>()->SetShape(rect);

    auto b = CreateAndGetObject();
    b->AddComponent<Collision::CollisionObject>()->SetActive(false);
    b->SetPosition({250, 50});
    b->AddComponent<Collision::RectCollider>()->SetShape(rect);
    b->AddComponent<Siv::Rect>()->SetShape(rect);

    auto c = CreateAndGetObject();
    c->AddComponent<Collision::CollisionObject>()->SetActive(false);
    c->SetPosition({450, 50});
    auto x = c->AddComponent<Collision::RectCollider>();
    x->SetShape(rect);
    x->SetActive(false);
    c->AddComponent<Siv::Rect>()->SetShape(rect);

    const s3d::Circle cir(0, 0, 40);
    const auto cur = CreateAndGetObject();
    cur->AddComponent<ComponentEngine::Siv::MouseChase>();
    cur->AddComponent<Collision::CollisionObject>();
    cur->AddComponent<Collision::CircleCollider>()->SetShape(cir);
    cur->AddComponent<Siv::Circle>()->SetShape(cir);
    cur->AddComponent<Hit>();

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