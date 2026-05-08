#include "OrderRepository.h"
#include <stdexcept>
#include <algorithm>

void OrderRepository::Save(const Order& order) {
    store_[order.orderId] = order;
}

Order OrderRepository::FindById(const std::string& id) {
    auto it = store_.find(id);
    if (it == store_.end()) throw std::runtime_error("Order not found: " + id);
    return it->second;
}

std::vector<Order> OrderRepository::FindAll() {
    std::vector<Order> result;
    result.reserve(store_.size());
    for (auto& [k, v] : store_) result.push_back(v);
    return result;
}

void OrderRepository::Update(const Order& order) {
    if (!store_.contains(order.orderId))
        throw std::runtime_error("Order not found: " + order.orderId);
    store_[order.orderId] = order;
}

void OrderRepository::Delete(const std::string& id) {
    store_.erase(id);
}

std::vector<Order> OrderRepository::FindByStatus(OrderStatus status) {
    std::vector<Order> result;
    for (auto& [k, v] : store_)
        if (v.status == status) result.push_back(v);
    return result;
}
