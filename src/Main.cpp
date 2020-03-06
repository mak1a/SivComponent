
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "ComponentEngine/ComponentEngine.hpp"
#include "NetworkTest.hpp"
#include "SivComponent/SivComponent.hpp"

using std::cout;
using std::endl;

void Main()
{
    //シーン生成
    ComponentEngine::Scene scene;

    //ゲームオブジェクト生成
    auto object = ComponentEngine::CreateGameObject();
    object->transform().name = "object";
    object->transform().pos = {400, 200};
    object->transform().SetRotateByAngle(-15);  //回転させておく

    // Rectコンポーネントを追加
    auto rect = object->AddComponent<ComponentEngine::Siv::Rect>();
    rect->Size = {100, 100};

    auto obj2 = ComponentEngine::CreateGameObject();
    obj2->transform().name = "obj2";
    obj2->transform().pos = {0, 300};  // y座標だけずらす

    //シーンにオブジェクトを追加
    scene.AddObject(object);
    //子オブジェクトにする
    object->AddChild(obj2);

    std::cout << "Engine Start" << std::endl;

    obj2->AddComponent<ComponentEngine::Siv::Text>();

    int k = 5;
    while (true)
    {
        scene.Update();
        //        std::cout << "" << std::endl;
        if (k-- < 0)
        {
            break;
        }
    }

    //    PhotonTest();

    while (System::Update())
    {
        scene.Update();
    }

    cout << "int Main() End." << endl;
}
