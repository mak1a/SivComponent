#define NO_S3D_USING
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "PhotonComponent/PhotonComponent.hpp"

#include "Matching.hpp"
#include "PhotonComponent/PhotonInitScene.hpp"
#include "Title.hpp"
#include "main/Game/Game.hpp"
#include "main/InstantiateEntry.hpp"

#include "test.hpp"

//#include <unicode/unistr.h>

using std::cout;
using std::endl;

void Main()
{
    ComponentEngine::SceneManager manager;

    // manager.RegisterScene<TestScene>("test");
    manager.RegisterScene<ComponentEngine::Photon::PhotonInitScene>("PhotonInitOnceLoad");
    manager.RegisterScene<TitleScene>("Title");
    manager.RegisterScene<Matching>("Matching");
    manager.RegisterScene<Game>("Game");

    //はやくこれを消したい
    InstantiateEntry_Game(manager.GetCommon());

    while (s3d::System::Update())
    {
        manager.UpdateCurrentScene();
    }

    manager.End();
}
