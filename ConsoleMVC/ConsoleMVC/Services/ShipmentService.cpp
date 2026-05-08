#include "ShipmentService.h"
#include "../Utils/Utils.h"

ShipmentService::ShipmentService(ShipmentRepository& shipRepo,
                                  InventoryService&   invSvc,
                                  OrderService&       ordSvc)
    : shipRepo_(shipRepo), invSvc_(invSvc), ordSvc_(ordSvc) {}

bool ShipmentService::ProcessShipment(const std::string& orderId) {
    Order o = ordSvc_.GetOrder(orderId);
    if (!invSvc_.CheckStock(o.sampleId, o.quantity))
        return false;

    invSvc_.Deduct(o.sampleId, o.quantity);

    Shipment s;
    s.shipmentId = GenerateId("SHP");
    s.orderId    = o.orderId;
    s.sampleId   = o.sampleId;
    s.quantity   = o.quantity;
    s.shippedAt  = GetTimestamp();
    shipRepo_.Save(s);

    ordSvc_.UpdateStatus(orderId, OrderStatus::SHIPPED);
    return true;
}

void ShipmentService::RetryPending(const std::string& orderId) {
    ProcessShipment(orderId);
}

std::vector<Shipment> ShipmentService::GetHistory() {
    return shipRepo_.FindAll();
}
