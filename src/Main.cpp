#define NO_S3D_USING
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "PhotonComponent/PhotonComponent.hpp"

#include "Matching.hpp"
#include "PhotonComponent/PhotonInitScene.hpp"
#include "Title.hpp"

#include <unicode/unistr.h>

using std::cout;
using std::endl;

class Game : public ComponentEngine::IScene
{
    void Setup() override
    {
        s3d::Print(U"Game");
    }
};

void Main()
{
    ComponentEngine::SceneManager manager;

    manager.RegisterScene<ComponentEngine::Photon::PhotonInitScene>("PhotonInitOnceLoad");
    manager.RegisterScene<TitleScene>("Title");
    manager.RegisterScene<Matching>("Matching");
    manager.RegisterScene<Game>("Game");

    while (s3d::System::Update())
    {
        manager.UpdateCurrentScene();
    }

    manager.End();
}
