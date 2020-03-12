#define NO_S3D_USING
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "PhotonComponent/PhotonComponent.hpp"

#include "Matching.hpp"
#include "PhotonComponent/PhotonInitScene.hpp"
#include "TestScene.hpp"
#include "Title.hpp"

#include <unicode/unistr.h>

using std::cout;
using std::endl;

void Main()
{
    ComponentEngine::SceneManager manager;

    manager.RegisterScene<PhotonComponent::PhotonInitScene>("PhotonInitOnceLoad");
    manager.RegisterScene<TitleScene>("Title");
    manager.RegisterScene<Matching>("Matching");

    while (s3d::System::Update())
    {
        manager.UpdateCurrentScene();
    }

    manager.End();
}
