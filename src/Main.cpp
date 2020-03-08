
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "NetworkTest.hpp"

#include "TestScene.hpp"

using std::cout;
using std::endl;

void Main()
{
    //シーン生成
    TestScene scene;

    ComponentEngine::SceneManager manager;
    manager.RegisterScene<TestScene>("Test");

    std::cout << "Engine Start" << std::endl;

    //    PhotonTest();

    while (System::Update())
    {
        manager.UpdateCurrentScene();
    }

    cout << "int Main() End." << endl;
}
