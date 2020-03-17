#pragma once

#include <chrono>
#include <functional>

namespace Utilities
{
    //関数呼び出しの時間を開ける関数
    class IntervalCall
    {
    private:
        long long lastcall;
        //ミリ秒指定
        int32_t interval;
        std::function<void()> func;

    public:
        using ClockType = std::chrono::milliseconds;

        IntervalCall(int32_t intervalMs, std::function<void()> _func)
            : interval(intervalMs)
            , func(_func)
        {
            lastcall = std::chrono::duration_cast<ClockType>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        void Run()
        {
            auto c = std::chrono::duration_cast<ClockType>(std::chrono::system_clock::now().time_since_epoch()).count();

            auto deltatime = c - lastcall;

            //時間経過していたら呼ぶ
            if (interval < deltatime)
            {
                func();

                lastcall = std::chrono::duration_cast<ClockType>(std::chrono::system_clock::now().time_since_epoch()).count();
            }
        }
    };
}  // namespace Utilities