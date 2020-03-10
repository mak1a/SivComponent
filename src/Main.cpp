#define NO_S3D_USING
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "PhotonComponent/PhotonComponent.hpp"

#include "PhotonComponent/PhotonInitScene.hpp"
#include "TestScene.hpp"
#include "Matching.hpp"

using std::cout;
using std::endl;

void Main()
{
    ComponentEngine::SceneManager manager;

    manager.RegisterScene<PhotonComponent::PhotonInitScene>("PhotonInitOnceLoad");
    manager.RegisterScene<TestScene>("Title");
    manager.RegisterScene<Matching>("Matching");
    

    std::cout << "Engine Start" << std::endl;

    while (s3d::System::Update())
    {
        manager.UpdateCurrentScene();
    }
    
    manager.End();

    cout << "int Main() End." << endl;
}
