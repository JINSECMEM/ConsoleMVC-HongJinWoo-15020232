#pragma once
#include "IRepository.h"
#include "../Models/Order.h"
#include <map>

class OrderRepository : public IRepository<Order> {
public:
    void               Save(const Order& order)         override;
    Order              FindById(const std::string& id)  override;
    std::vector<Order> FindAll()                        override;
    void               Update(const Order& order)       override;
    void               Delete(const std::string& id)    override;

    std::vector<Order> FindByStatus(OrderStatus status);

private:
    std::map<std::string, Order> store_;
};
