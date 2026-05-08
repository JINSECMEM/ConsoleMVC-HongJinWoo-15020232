#include "InventoryService.h"
#include "../Utils/Utils.h"
#include <stdexcept>

InventoryService::InventoryService(InventoryRepository& repo) : repo_(repo) {}

bool InventoryService::CheckStock(const std::string& sampleId, int requiredQty) {
    return repo_.FindById(sampleId).stock >= requiredQty;
}

void InventoryService::Deduct(const std::string& sampleId, int qty) {
    Inventory inv = repo_.FindById(sampleId);
    if (inv.stock < qty)
        throw std::runtime_error("재고 부족: " + sampleId);
    inv.stock      -= qty;
    inv.lastUpdated = GetTimestamp();
    repo_.Update(inv);
}

void InventoryService::Replenish(const std::string& sampleId, int qty) {
    Inventory inv   = repo_.FindById(sampleId);
    inv.stock      += qty;
    inv.lastUpdated = GetTimestamp();
    repo_.Update(inv);
}

bool InventoryService::IsBelowThreshold(const std::string& sampleId) {
    Inventory inv = repo_.FindById(sampleId);
    return inv.stock <= inv.threshold;
}

int InventoryService::GetStock(const std::string& sampleId) {
    return repo_.FindById(sampleId).stock;
}

std::vector<Inventory> InventoryService::GetAll() {
    return repo_.FindAll();
}
