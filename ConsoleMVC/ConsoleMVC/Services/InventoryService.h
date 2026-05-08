#pragma once
#include "../Database/InventoryRepository.h"
#include <string>
#include <vector>

class InventoryService {
public:
    explicit InventoryService(InventoryRepository& repo);

    bool                   CheckStock(const std::string& sampleId, int requiredQty);
    void                   Deduct(const std::string& sampleId, int qty);
    void                   Replenish(const std::string& sampleId, int qty);
    bool                   IsBelowThreshold(const std::string& sampleId);
    int                    GetStock(const std::string& sampleId);
    std::vector<Inventory> GetAll();

private:
    InventoryRepository& repo_;
};
