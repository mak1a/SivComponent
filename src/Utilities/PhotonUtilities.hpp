#pragma once

#define NO_S3D_USING
#include <Common-cpp/inc/JString.h>

namespace Utilities
{
    // namespace ExitGames::Common
    // {
    //     class JString;
    // }
    // namespace s3d
    // {
    //     class String;
    // }
    int GetPlayerNumber(const ExitGames::Common::JString& name);
    //    int GetPlayerNumber(const s3d::String& name);

}  // namespace Utilities
