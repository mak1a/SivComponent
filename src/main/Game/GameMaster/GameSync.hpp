#pragma once

#include <Common-cpp/inc/Common.h>
#include "../../CustomEventList.hpp"

namespace DataName::GameSync
{
    constexpr nByte Result = 1;

    //結果の種類
    enum class ResultType
    {
        Victory,
        Defeat,
        Drawn,
        Timeup,
    };
}  // namespace DataName::GameSync

void SendDefeatSignal();
void SendVictorySignal();