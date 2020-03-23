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

        IntervalCall()
            : IntervalCall(-1, []() {})
        {
        }

        bool IsStop()
        {
            return interval < 0;
        }

        void Stop()
        {
            interval = -1;
        }

        void ResetInterval(int32_t intervalMs)
        {
            interval = intervalMs;
            lastcall = std::chrono::duration_cast<ClockType>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        void Run()
        {
            if (IsStop())
            {
                return;
            }

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