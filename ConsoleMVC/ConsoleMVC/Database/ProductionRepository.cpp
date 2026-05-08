#include "ProductionRepository.h"
#include <stdexcept>

ProductionRepository::ProductionRepository() {
    for (auto& l : std::vector<ProductionLine>{
        { "LINE-A", "S-001", LineStatus::IDLE, "", 0 },
        { "LINE-B", "S-002", LineStatus::IDLE, "", 0 },
        { "LINE-C", "S-003", LineStatus::IDLE, "", 0 },
    }) lines_[l.lineId] = l;
}

ProductionLine ProductionRepository::FindLineById(const std::string& lineId) {
    auto it = lines_.find(lineId);
    if (it == lines_.end()) throw std::runtime_error("Line not found: " + lineId);
    return it->second;
}
std::vector<ProductionLine> ProductionRepository::FindAllLines() {
    std::vector<ProductionLine> r; r.reserve(lines_.size());
    for (auto& [k, v] : lines_) r.push_back(v);
    return r;
}
void ProductionRepository::UpdateLine(const ProductionLine& line) {
    lines_[line.lineId] = line;
}

void ProductionRepository::SaveJob(const ProductionJob& job)  { jobs_[job.jobId] = job; }
ProductionJob ProductionRepository::FindJobById(const std::string& jobId) {
    auto it = jobs_.find(jobId);
    if (it == jobs_.end()) throw std::runtime_error("Job not found: " + jobId);
    return it->second;
}
std::vector<ProductionJob> ProductionRepository::FindAllJobs() {
    std::vector<ProductionJob> r; r.reserve(jobs_.size());
    for (auto& [k, v] : jobs_) r.push_back(v);
    return r;
}
void ProductionRepository::UpdateJob(const ProductionJob& job) { jobs_[job.jobId] = job; }
