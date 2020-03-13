
#define NO_S3D_USING
#include "Utilities.hpp"

#include <cmath>

namespace Utilities
{
    int GetPlayerNumber(const ExitGames::Common::JString& name)
    {
        auto sub = name.substring(6);
        return sub.toInt();
    }
    // int GetPlayerNumber(const s3d::String& name) {}
}  // namespace Utilities
