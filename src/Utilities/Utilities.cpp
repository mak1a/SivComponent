
#include "Utilities.hpp"

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
}  // namespace Utilities