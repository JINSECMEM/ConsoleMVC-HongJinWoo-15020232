#pragma once
#include "../Models/ProductionLine.h"
#include "../Models/ProductionJob.h"
#include <vector>

class ProductionView {
public:
    void RenderLines(const std::vector<ProductionLine>& lines);
    void RenderJobs(const std::vector<ProductionJob>& jobs);
};
