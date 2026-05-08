#pragma once
#include <string>

enum class OrderStatus {
    PENDING,
    CHECKING,
    READY_TO_SHIP,
    AWAITING_PROD,
    IN_PRODUCTION,
    SHIPPED,
    CANCELLED
};

inline const char* OrderStatusStr(OrderStatus s) {
    switch (s) {
    case OrderStatus::PENDING:       return "PENDING";
    case OrderStatus::CHECKING:      return "CHECKING";
    case OrderStatus::READY_TO_SHIP: return "READY_TO_SHIP";
    case OrderStatus::AWAITING_PROD: return "AWAITING_PROD";
    case OrderStatus::IN_PRODUCTION: return "IN_PRODUCTION";
    case OrderStatus::SHIPPED:       return "SHIPPED";
    case OrderStatus::CANCELLED:     return "CANCELLED";
    default:                         return "UNKNOWN";
    }
}

struct Order {
    std::string orderId;
    std::string customerId;
    std::string sampleId;
    int         quantity  = 0;
    std::string dueDate;
    OrderStatus status    = OrderStatus::PENDING;
    std::string createdAt;
    std::string updatedAt;
};
