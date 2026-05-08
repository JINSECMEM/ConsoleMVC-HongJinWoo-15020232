#pragma once
#include "../Database/OrderRepository.h"
#include <string>
#include <vector>

class OrderService {
public:
    explicit OrderService(OrderRepository& repo);

    Order              RegisterOrder(const std::string& customerId,
                                     const std::string& sampleId,
                                     int qty,
                                     const std::string& dueDate);
    bool               CancelOrder(const std::string& orderId);
    void               UpdateStatus(const std::string& orderId, OrderStatus status);
    Order              GetOrder(const std::string& orderId);
    std::vector<Order> GetAllOrders();

private:
    OrderRepository& repo_;
};
