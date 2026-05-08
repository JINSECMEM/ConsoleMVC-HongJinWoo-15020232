#pragma once
#include "IRepository.h"
#include "../Models/Shipment.h"
#include <map>

class ShipmentRepository : public IRepository<Shipment> {
public:
    void                   Save(const Shipment& s)         override;
    Shipment               FindById(const std::string& id) override;
    std::vector<Shipment>  FindAll()                       override;
    void                   Update(const Shipment& s)       override;
    void                   Delete(const std::string& id)   override;

private:
    std::map<std::string, Shipment> store_;
};
