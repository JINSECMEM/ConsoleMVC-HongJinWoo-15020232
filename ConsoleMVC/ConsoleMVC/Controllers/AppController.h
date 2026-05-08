#pragma once
#include "../Database/OrderRepository.h"
#include "../Database/SampleRepository.h"
#include "../Database/InventoryRepository.h"
#include "../Database/ProductionRepository.h"
#include "../Database/ShipmentRepository.h"
#include "../Services/OrderService.h"
#include "../Services/InventoryService.h"
#include "../Services/ProductionService.h"
#include "../Services/ShipmentService.h"
#include "../Views/MenuView.h"
#include "../Views/OrderView.h"
#include "../Views/InventoryView.h"
#include "../Views/ProductionView.h"
#include "../Views/ShipmentView.h"
#include "../Views/MonitorView.h"
#include "OrderController.h"
#include "InventoryController.h"
#include "ProductionController.h"
#include "ShipmentController.h"
#include <vector>
#include <string>

class AppController {
public:
    AppController();
    void Run();

private:
    void HandleMenu(int choice);
    void ShowMonitor();
    void Tick();
    void AddLog(const std::string& msg);

    // ── Repositories (선언 순서 = 초기화 순서) ─────────────
    OrderRepository      orderRepo_;
    SampleRepository     sampleRepo_;
    InventoryRepository  invRepo_;
    ProductionRepository prodRepo_;
    ShipmentRepository   shipRepo_;

    // ── Services ──────────────────────────────────────────
    OrderService      orderSvc_;
    InventoryService  invSvc_;
    ProductionService prodSvc_;
    ShipmentService   shipSvc_;

    // ── Views ─────────────────────────────────────────────
    MenuView       menuView_;
    OrderView      orderView_;
    InventoryView  invView_;
    ProductionView prodView_;
    ShipmentView   shipView_;
    MonitorView    monitorView_;

    // ── Controllers ───────────────────────────────────────
    OrderController      orderCtrl_;
    InventoryController  invCtrl_;
    ProductionController prodCtrl_;
    ShipmentController   shipCtrl_;

    std::vector<std::string> eventLog_;
    bool running_ = true;
};
