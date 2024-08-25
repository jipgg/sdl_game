#pragma once
#include <common.h>
#include <functional>
#include <chrono>

class Time_interval {
    std::chrono::steady_clock::time_point last_time;
public:
    const std::chrono::milliseconds interval;
    Time_interval(std::chrono::milliseconds interval):
        last_time(std::chrono::steady_clock::now()),
        interval(interval) {}
    void operator()(std::function<void()>&& codeblock) {
        auto curr_time = std::chrono::steady_clock::now();
        std::chrono::milliseconds delta_ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - last_time);
        if (delta_ms >= interval) {
            last_time = curr_time;
            codeblock();
        }
    }
};
