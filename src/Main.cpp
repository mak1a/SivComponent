
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "ComponentEngine/ComponentEngine.hpp"

using std::cout;
using std::endl;

namespace ComponentEngine::siv
{
    class Circle : public ComponentEngine::AttachableComponent
    {
        void Update() override
        {
//            s3d::Circle(this->, 100, 50).draw();
        }
    };
}


void Main()
{
    //シーン生成
    ComponentEngine::Scene scene;
    
    //ゲームオブジェクト生成
    auto object = ComponentEngine::CreateGameObject();
    object->transform().name = "object";
    
    //コンポーネントを追加
    object->AddComponent<ComponentEngine::siv::Circle>();
    
    
    auto obj2 = ComponentEngine::CreateGameObject();
    obj2->transform().name = "obj2";
    obj2->AddComponent<ComponentEngine::siv::Circle>();
    
    //シーンにオブジェクトを追加
    scene.AddObject(object);
    //子オブジェクトにする
    object->AddChild(obj2);
    
    std::cout << "Engine Start" << std::endl;
    
    //StartとUpdateが呼ばれる
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
        if(k-- < 0)
        {
            break;
        }
    }
    
    while(System::Update())
    {
        scene.Update();
    }
}
