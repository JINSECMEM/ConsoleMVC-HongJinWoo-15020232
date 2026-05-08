#pragma once
#include "IRepository.h"
#include "../Models/Inventory.h"
#include <map>

class InventoryRepository : public IRepository<Inventory> {
public:
    InventoryRepository();  // 초기 재고 적재

    void                   Save(const Inventory& inv)      override;
    Inventory              FindById(const std::string& id) override;
    std::vector<Inventory> FindAll()                       override;
    void                   Update(const Inventory& inv)    override;
    void                   Delete(const std::string& id)   override;

private:
    std::map<std::string, Inventory> store_;
};
