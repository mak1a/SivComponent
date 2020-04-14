#define NO_S3D_USING
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "PhotonComponent/PhotonComponent.hpp"

#include "PhotonComponent/PhotonInit.hpp"
#include "Title.hpp"
#include "main/AssetLoad.hpp"
#include "main/Game/Game.hpp"
#include "main/InstantiateEntry.hpp"
#include "main/Matching/Matching.hpp"
#include "main/PlayerCustom/PlayerCustom.hpp"

#include "test.hpp"

//#include <unicode/unistr.h>

using std::cout;
using std::endl;

void Main()
{
    ComponentEngine::SceneManager manager;

    Photon::PhotonInit(manager);

//    manager.RegisterScene<TestScene>("test");
    manager.RegisterScene<TitleScene>("Title");
    manager.RegisterScene<PlayerCustom>("Custom");
    manager.RegisterScene<Matching>("Matching");
    manager.RegisterScene<Game>("Game");

    //はやくこれを消したい
    InstantiateEntry_Game(manager.GetCommon());
    LoadAssets();

    while (s3d::System::Update())
    {
        manager.UpdateCurrentScene();
    }

    manager.End();
}
