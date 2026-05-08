#pragma once
#include "../Database/ProductionRepository.h"
#include <functional>
#include <string>
#include <vector>

class ProductionService {
public:
    explicit ProductionService(ProductionRepository& repo);

    using OnCompleteCallback = std::function<void(const ProductionJob&)>;
    void SetOnCompleteCallback(OnCompleteCallback cb);

    // 생산 요청: 해당 시료의 전담 라인에 Job 등록
    ProductionJob               RequestProduction(const std::string& sampleId,
                                                  int qty,
                                                  const std::string& orderId);

    // AppLoop에서 1초마다 호출 — 진행률 +TICK_PCT, 완료 시 콜백 발동
    void                        Tick();

    std::vector<ProductionLine> GetAllLines();
    std::vector<ProductionJob>  GetAllJobs();

private:
    static constexpr int TICK_PCT = 20; // 5틱(5초)에 생산 완료

    ProductionRepository& repo_;
    OnCompleteCallback    onComplete_;
};
