#include "OrderController.h"
#include "../Views/ConsoleHelper.h"
#include <iostream>

OrderController::OrderController(OrderService&      ordSvc,
                                  InventoryService&  invSvc,
                                  ProductionService& prodSvc,
                                  ShipmentService&   shipSvc,
                                  SampleRepository&  sampleRepo,
                                  OrderView&         view,
                                  std::function<void(const std::string&)> addLog)
    : ordSvc_(ordSvc), invSvc_(invSvc), prodSvc_(prodSvc),
      shipSvc_(shipSvc), sampleRepo_(sampleRepo), view_(view), addLog_(addLog) {}

void OrderController::RegisterOrder() {
    auto samples = sampleRepo_.FindAll();
    OrderInputDto dto = view_.GetOrderInput(samples);

    if (dto.sampleId.empty() || dto.quantity <= 0) {
        std::cout << "\n  [오류] 잘못된 입력입니다.\n";
        ConsoleHelper::Pause();
        return;
    }

    Order order = ordSvc_.RegisterOrder(dto.customerId, dto.sampleId,
                                         dto.quantity, dto.dueDate);
    addLog_("주문 등록: " + order.orderId + "  " + order.sampleId +
            "  " + std::to_string(order.quantity) + "개");

    ordSvc_.UpdateStatus(order.orderId, OrderStatus::CHECKING);

    bool hasStock = invSvc_.CheckStock(order.sampleId, order.quantity);
    if (hasStock) {
        shipSvc_.ProcessShipment(order.orderId);
        order = ordSvc_.GetOrder(order.orderId);
        addLog_("즉시 출고: " + order.orderId);
        view_.RenderOrderResult(order, true, "재고 충분 → 즉시 출고 완료");
    } else {
        ordSvc_.UpdateStatus(order.orderId, OrderStatus::AWAITING_PROD);
        prodSvc_.RequestProduction(order.sampleId, order.quantity, order.orderId);
        ordSvc_.UpdateStatus(order.orderId, OrderStatus::IN_PRODUCTION);
        order = ordSvc_.GetOrder(order.orderId);
        addLog_("생산 요청: " + order.orderId + " → " + order.sampleId + " 생산라인 가동");
        view_.RenderOrderResult(order, false, "재고 부족 → 생산 요청 완료 (생산 진행 중)");
    }

    if (invSvc_.IsBelowThreshold(order.sampleId)) {
        ConsoleHelper::SetColor(ConsoleHelper::COLOR_RED);
        std::cout << "\n  [경고] " << order.sampleId << " 재고가 임계값 이하입니다.\n";
        ConsoleHelper::ResetColor();
        addLog_("[경고] " + order.sampleId + " 재고 임계값 이하");
    }

    ConsoleHelper::Pause();
}

void OrderController::ListOrders() {
    view_.RenderOrderList(ordSvc_.GetAllOrders());
    ConsoleHelper::Pause();
}

void OrderController::CancelOrder() {
    std::string id = view_.GetOrderIdInput();
    bool ok = ordSvc_.CancelOrder(id);
    if (ok) {
        ConsoleHelper::SetColor(ConsoleHelper::COLOR_GREEN);
        std::cout << "  [완료] 주문 취소: " << id << '\n';
        addLog_("주문 취소: " + id);
    } else {
        ConsoleHelper::SetColor(ConsoleHelper::COLOR_RED);
        std::cout << "  [실패] 취소 불가 (이미 출고됐거나 취소된 주문)\n";
    }
    ConsoleHelper::ResetColor();
    ConsoleHelper::Pause();
}
