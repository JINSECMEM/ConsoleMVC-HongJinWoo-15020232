#include "SampleRepository.h"
#include <stdexcept>

SampleRepository::SampleRepository() {
    for (auto& s : std::vector<Sample>{
        { "S-001", "DRAM-8Gb",    "LINE-A" },
        { "S-002", "NAND-128Gb",  "LINE-B" },
        { "S-003", "Logic-SOC",   "LINE-C" },
    }) store_[s.sampleId] = s;
}

void SampleRepository::Save(const Sample& s)           { store_[s.sampleId] = s; }
Sample SampleRepository::FindById(const std::string& id) {
    auto it = store_.find(id);
    if (it == store_.end()) throw std::runtime_error("Sample not found: " + id);
    return it->second;
}
std::vector<Sample> SampleRepository::FindAll() {
    std::vector<Sample> r; r.reserve(store_.size());
    for (auto& [k, v] : store_) r.push_back(v);
    return r;
}
void SampleRepository::Update(const Sample& s) { store_[s.sampleId] = s; }
void SampleRepository::Delete(const std::string& id) { store_.erase(id); }
