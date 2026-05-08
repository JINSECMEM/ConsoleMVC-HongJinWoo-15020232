#include "OrderService.h"
#include "../Utils/Utils.h"

OrderService::OrderService(OrderRepository& repo) : repo_(repo) {}

Order OrderService::RegisterOrder(const std::string& customerId,
                                   const std::string& sampleId,
                                   int qty,
                                   const std::string& dueDate) {
    Order o;
    o.orderId    = GenerateId("ORD");
    o.customerId = customerId;
    o.sampleId   = sampleId;
    o.quantity   = qty;
    o.dueDate    = dueDate;
    o.status     = OrderStatus::PENDING;
    o.createdAt  = GetTimestamp();
    o.updatedAt  = o.createdAt;
    repo_.Save(o);
    return o;
}

bool OrderService::CancelOrder(const std::string& orderId) {
    Order o = repo_.FindById(orderId);
    if (o.status == OrderStatus::SHIPPED || o.status == OrderStatus::CANCELLED)
        return false;
    o.status    = OrderStatus::CANCELLED;
    o.updatedAt = GetTimestamp();
    repo_.Update(o);
    return true;
}

void OrderService::UpdateStatus(const std::string& orderId, OrderStatus status) {
    Order o     = repo_.FindById(orderId);
    o.status    = status;
    o.updatedAt = GetTimestamp();
    repo_.Update(o);
}

Order OrderService::GetOrder(const std::string& orderId) {
    return repo_.FindById(orderId);
}

std::vector<Order> OrderService::GetAllOrders() {
    return repo_.FindAll();
}
