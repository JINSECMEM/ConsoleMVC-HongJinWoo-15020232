#pragma once
#include <string>

struct Inventory {
    std::string sampleId;
    int         stock     = 0;
    int         threshold = 0;  // 안전 재고 임계값
    std::string lastUpdated;
};
