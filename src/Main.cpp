#define NO_S3D_USING
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "PhotonComponent/PhotonComponent.hpp"

#include "TestScene.hpp"

using std::cout;
using std::endl;

class MatchSystem : public ComponentEngine::AttachableComponent
{
    void Start () override
    {
        auto system = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommonObject("PhotonSystem");
        system->GetComponent<PhotonComponent::NetworkSystem>()->Connect();
    }
};

class Matching : public ComponentEngine::IScene
{
    void Setup()
    {
        CreateAndGetObject()->AddComponent<MatchSystem>();
    }
};

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

    cout << "int Main() End." << endl;
}
