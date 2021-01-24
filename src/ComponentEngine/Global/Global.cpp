#include "Global.hpp"

namespace ComponentEngine::Global
{
    namespace
    {
        // 現状OnExitCollisionで使用している
        int frameCount = 0;
    }

    void Update() {
        ++frameCount;
    }

    int FrameCount() {
        return frameCount;
    }
}
