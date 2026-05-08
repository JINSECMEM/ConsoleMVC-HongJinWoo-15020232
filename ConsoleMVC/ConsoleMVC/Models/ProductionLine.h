#pragma once
#include <string>

enum class LineStatus { IDLE, RUNNING, PAUSED, MAINTENANCE };

inline const char* LineStatusStr(LineStatus s) {
    switch (s) {
    case LineStatus::IDLE:        return "IDLE";
    case LineStatus::RUNNING:     return "RUNNING";
    case LineStatus::PAUSED:      return "PAUSED";
    case LineStatus::MAINTENANCE: return "MAINTENANCE";
    default:                      return "UNKNOWN";
    }
}

struct ProductionLine {
    std::string lineId;
    std::string sampleId;
    LineStatus  status       = LineStatus::IDLE;
    std::string currentJobId;   // 빈 문자열이면 작업 없음
    int         progressPct  = 0;
};
