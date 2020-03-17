#pragma once

#include <chrono>
#include <functional>

// template <typename... Args>
namespace Utilities
{
    //関数呼び出しの時間を開ける関数
    class IntervalCall
    {
    private:
        long long lastcall;
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

        // static double DeltaTime()
        // {
        //     return deltatime / (1000 * 1000 * 1000.0);
        // }

        // static void Update()
        // {
        //     bool call_once = [&]() {
        //         deltatime = 0;
        //         return true;
        //     }();

        //     static auto current = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        //     auto c = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        //     deltatime = c - current;

        //     current = c;

        //     static long long sum = 0;
        //     sum += deltatime;

        //     // std::cout << DeltaTime() << "sum:" << sum << std::endl;
        // }
    };
}  // namespace Utilities