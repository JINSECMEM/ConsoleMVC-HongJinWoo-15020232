#include "InventoryRepository.h"
#include "../Utils/Utils.h"
#include <stdexcept>

InventoryRepository::InventoryRepository() {
    // S-002는 임계값(500) 미달 상태로 시작 — 경고 시나리오 시연용
    for (auto& inv : std::vector<Inventory>{
        { "S-001", 1000, 500, GetTimestamp() },
        { "S-002",  300, 500, GetTimestamp() },
        { "S-003",  800, 300, GetTimestamp() },
    }) store_[inv.sampleId] = inv;
}

void InventoryRepository::Save(const Inventory& inv)       { store_[inv.sampleId] = inv; }
Inventory InventoryRepository::FindById(const std::string& id) {
    auto it = store_.find(id);
    if (it == store_.end()) throw std::runtime_error("Inventory not found: " + id);
    return it->second;
}
std::vector<Inventory> InventoryRepository::FindAll() {
    std::vector<Inventory> r; r.reserve(store_.size());
    for (auto& [k, v] : store_) r.push_back(v);
    return r;
}
void InventoryRepository::Update(const Inventory& inv) {
    if (!store_.contains(inv.sampleId))
        throw std::runtime_error("Inventory not found: " + inv.sampleId);
    store_[inv.sampleId] = inv;
}
void InventoryRepository::Delete(const std::string& id) { store_.erase(id); }
