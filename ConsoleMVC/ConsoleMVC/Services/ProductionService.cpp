#include "ProductionService.h"
#include "../Utils/Utils.h"
#include <stdexcept>

ProductionService::ProductionService(ProductionRepository& repo) : repo_(repo) {}

void ProductionService::SetOnCompleteCallback(OnCompleteCallback cb) {
    onComplete_ = std::move(cb);
}

ProductionJob ProductionService::RequestProduction(const std::string& sampleId,
                                                    int qty,
                                                    const std::string& orderId) {
    // 시료별 전담 라인 조회
    ProductionLine line;
    for (auto& l : repo_.FindAllLines())
        if (l.sampleId == sampleId) { line = l; break; }

    if (line.lineId.empty())
        throw std::runtime_error("생산라인 없음: " + sampleId);
    if (line.status == LineStatus::MAINTENANCE)
        throw std::runtime_error("라인 점검 중 (생산 불가): " + line.lineId);

    ProductionJob job;
    job.jobId     = GenerateId("JOB");
    job.orderId   = orderId;
    job.sampleId  = sampleId;
    job.lineId    = line.lineId;
    job.targetQty = qty;
    job.status    = JobStatus::RUNNING;
    job.startedAt = GetTimestamp();
    repo_.SaveJob(job);

    // 라인 상태 갱신
    line.status       = LineStatus::RUNNING;
    line.currentJobId = job.jobId;
    line.progressPct  = 0;
    repo_.UpdateLine(line);

    return job;
}

void ProductionService::Tick() {
    for (auto line : repo_.FindAllLines()) {
        if (line.status != LineStatus::RUNNING) continue;

        line.progressPct = std::min(100, line.progressPct + TICK_PCT);
        repo_.UpdateLine(line);

        if (line.progressPct >= 100) {
            // 작업 완료 처리
            ProductionJob job      = repo_.FindJobById(line.currentJobId);
            job.completedQty       = job.targetQty;
            job.status             = JobStatus::COMPLETED;
            job.completedAt        = GetTimestamp();
            repo_.UpdateJob(job);

            // 라인 초기화
            line.status       = LineStatus::IDLE;
            line.currentJobId = "";
            line.progressPct  = 0;
            repo_.UpdateLine(line);

            if (onComplete_) onComplete_(job);
        }
    }
}

std::vector<ProductionLine> ProductionService::GetAllLines() { return repo_.FindAllLines(); }
std::vector<ProductionJob>  ProductionService::GetAllJobs()  { return repo_.FindAllJobs(); }
