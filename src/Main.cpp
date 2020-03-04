
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "ComponentEngine/ComponentEngine.hpp"

using std::cout;
using std::endl;
using namespace s3d;

namespace ComponentEngine::siv
{
    class Circle : public ComponentEngine::AttachableComponent
    {
    protected:
        void Update() override
        {
            s3d::Circle(0, 0, r).draw();
        }

    public:
        double r = 50;
    };
}  // namespace ComponentEngine::siv

void Main()
{
    //シーン生成
    ComponentEngine::Scene scene;

    //ゲームオブジェクト生成
    auto object = ComponentEngine::CreateGameObject();
    object->transform().name = "object";
    object->transform().pos = {400, 200};
    object->transform().SetRotateByAngle(-15);
    object->transform().scale = {0.6, 0.6};

    //コンポーネントを追加
    object->AddComponent<ComponentEngine::siv::Circle>();

    auto obj2 = ComponentEngine::CreateGameObject();
    obj2->transform().name = "obj2";
    obj2->AddComponent<ComponentEngine::siv::Circle>();
    obj2->transform().pos = {0, 300};

    //シーンにオブジェクトを追加
    scene.AddObject(object);
    //子オブジェクトにする
    object->AddChild(obj2);

    std::cout << "Engine Start" << std::endl;

    // StartとUpdateが呼ばれる
    scene.Update();

    std::cout << "\nAdd Component Event" << std::endl;
    //    //途中でコンポーネントを追加
    //    obj2->AddComponent<SampleComponent>();
    //    obj2->AddComponent<OverrideComponent>();

    int k = 5;
    while (true)
    {
        scene.Update();
        std::cout << "" << std::endl;
        if (k-- < 0)
        {
            break;
        }
    }

    while (System::Update())
    {
        scene.Update();
    }
}
