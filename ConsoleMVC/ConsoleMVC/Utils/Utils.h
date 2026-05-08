#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <atomic>

inline std::string GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto tt  = std::chrono::system_clock::to_time_t(now);
    std::tm tm{}; localtime_s(&tm, &tt);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

inline std::string GetDateStr() {
    auto now = std::chrono::system_clock::now();
    auto tt  = std::chrono::system_clock::to_time_t(now);
    std::tm tm{}; localtime_s(&tm, &tt);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d");
    return oss.str();
}

// prefix: "ORD" / "JOB" / "SHP"
inline std::string GenerateId(const std::string& prefix) {
    static std::atomic<int> counter{ 0 };
    std::ostringstream oss;
    oss << prefix << "-" << GetDateStr()
        << "-" << std::setw(4) << std::setfill('0') << ++counter;
    return oss.str();
}
