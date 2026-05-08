#pragma once
#include "../Services/OrderService.h"
#include "../Services/InventoryService.h"
#include "../Services/ProductionService.h"
#include "../Services/ShipmentService.h"
#include "../Database/SampleRepository.h"
#include "../Views/OrderView.h"
#include <functional>
#include <string>

class OrderController {
public:
    OrderController(OrderService&      ordSvc,
                    InventoryService&  invSvc,
                    ProductionService& prodSvc,
                    ShipmentService&   shipSvc,
                    SampleRepository&  sampleRepo,
                    OrderView&         view,
                    std::function<void(const std::string&)> addLog);

    void RegisterOrder();
    void ListOrders();
    void CancelOrder();

private:
    OrderService&      ordSvc_;
    InventoryService&  invSvc_;
    ProductionService& prodSvc_;
    ShipmentService&   shipSvc_;
    SampleRepository&  sampleRepo_;
    OrderView&         view_;
    std::function<void(const std::string&)> addLog_;
};
