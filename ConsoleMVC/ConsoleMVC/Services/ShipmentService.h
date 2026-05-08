#pragma once
#include "../Database/ShipmentRepository.h"
#include "InventoryService.h"
#include "OrderService.h"
#include <vector>

class ShipmentService {
public:
    ShipmentService(ShipmentRepository& shipRepo,
                    InventoryService&   invSvc,
                    OrderService&       ordSvc);

    // 재고 차감 후 Shipment 생성, 주문 상태 SHIPPED 로 변경
    bool                  ProcessShipment(const std::string& orderId);

    // 생산 완료 후 대기 주문 재처리
    void                  RetryPending(const std::string& orderId);

    std::vector<Shipment> GetHistory();

private:
    ShipmentRepository& shipRepo_;
    InventoryService&   invSvc_;
    OrderService&       ordSvc_;
};
