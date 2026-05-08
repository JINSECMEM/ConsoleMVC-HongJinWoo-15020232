#include "ShipmentRepository.h"
#include <stdexcept>

void ShipmentRepository::Save(const Shipment& s)       { store_[s.shipmentId] = s; }
Shipment ShipmentRepository::FindById(const std::string& id) {
    auto it = store_.find(id);
    if (it == store_.end()) throw std::runtime_error("Shipment not found: " + id);
    return it->second;
}
std::vector<Shipment> ShipmentRepository::FindAll() {
    std::vector<Shipment> r; r.reserve(store_.size());
    for (auto& [k, v] : store_) r.push_back(v);
    return r;
}
void ShipmentRepository::Update(const Shipment& s) { store_[s.shipmentId] = s; }
void ShipmentRepository::Delete(const std::string& id) { store_.erase(id); }
