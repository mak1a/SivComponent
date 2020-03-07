
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
    object->transform().SetName("object");
    object->transform().SetPosition({400, 200});
    object->transform().SetRotateByAngle(-15);  //回転させておく

    // Rectコンポーネントを追加
    auto rect = object->AddComponent<ComponentEngine::Siv::Rect>();
    rect->Size = {100, 100};
    //シーンにオブジェクトを追加
    scene.AddObject(object);

    auto obj2 = ComponentEngine::CreateGameObject();
    obj2->transform().SetName("obj2");
    obj2->transform().SetPosition({0, 300});  // y座標だけずらす
    obj2->AddComponent<ComponentEngine::Siv::Text>();
    //子オブジェクトにする
    object->AddChild(obj2);

    //マウス追従オブジェクト
    auto mouseObj = ComponentEngine::CreateGameObject();
    mouseObj->AddComponent<ComponentEngine::Siv::Circle>()->SetColor(s3d::Palette::Blue).SetR(30);
    mouseObj->AddComponent<ComponentEngine::Siv::MouseChase>();
    scene.AddObject(mouseObj);

    std::cout << "Engine Start" << std::endl;

    PhotonTest();
    
    //    while (System::Update())
    //    {
    //        scene.Update();
    //    }

    cout << "int Main() End." << endl;
}
