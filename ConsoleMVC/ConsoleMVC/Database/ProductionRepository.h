#pragma once
#include "IRepository.h"
#include "../Models/ProductionLine.h"
#include "../Models/ProductionJob.h"
#include <map>

class ProductionRepository {
public:
    ProductionRepository();  // 생산라인 3개 초기 적재

    // ProductionLine CRUD
    ProductionLine              FindLineById(const std::string& lineId);
    std::vector<ProductionLine> FindAllLines();
    void                        UpdateLine(const ProductionLine& line);

    // ProductionJob CRUD
    void                        SaveJob(const ProductionJob& job);
    ProductionJob               FindJobById(const std::string& jobId);
    std::vector<ProductionJob>  FindAllJobs();
    void                        UpdateJob(const ProductionJob& job);

private:
    std::map<std::string, ProductionLine> lines_;
    std::map<std::string, ProductionJob>  jobs_;
};
