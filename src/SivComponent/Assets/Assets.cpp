
#include "Assets.hpp"
using namespace s3d;

namespace ComponentEngine::Siv
{
    BGM::BGM(const String& audioName)
        : name(audioName)
    {
    }

    void BGM::Awake()
    {
        AudioAsset(name).play();
    }

    void BGM::OnDestroy()
    {
        AudioAsset(name).stop();
    }

}  // namespace ComponentEngine::Siv
