#pragma once
#include <string>

enum class JobStatus { QUEUED, RUNNING, COMPLETED, FAILED };

struct ProductionJob {
    std::string jobId;
    std::string orderId;
    std::string sampleId;
    std::string lineId;
    int         targetQty    = 0;
    int         completedQty = 0;
    JobStatus   status       = JobStatus::QUEUED;
    std::string startedAt;
    std::string completedAt;
};
