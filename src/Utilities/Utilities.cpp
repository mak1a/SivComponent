#define _USE_MATH_DEFINES
#include "Utilities.hpp"
#include <cmath>

namespace Utilities
{
    double DegToRad(double deg)
    {
        return deg * M_PI / 180.0;
    }

    double RadToDeg(double rad)
    {
        return rad * 180.0 / M_PI;
    }
    // int GetPlayerNumber(const s3d::String& name) {}
}  // namespace Utilities
