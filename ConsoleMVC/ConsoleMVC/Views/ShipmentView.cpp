#include "ShipmentView.h"
#include "ConsoleHelper.h"
#include <iostream>

void ShipmentView::RenderHistory(const std::vector<Shipment>& shipments) {
    ConsoleHelper::PrintDivider('=');
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_YELLOW);
    std::cout << "  출고 이력 (" << shipments.size() << "건)\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider();

    if (shipments.empty()) { std::cout << "  (출고 내역 없음)\n"; return; }

    std::vector<std::vector<std::string>> rows;
    for (auto& s : shipments)
        rows.push_back({ s.shipmentId, s.orderId, s.sampleId,
                         std::to_string(s.quantity), s.shippedAt });
    ConsoleHelper::PrintTable(
        { "출고ID", "주문ID", "시료", "수량", "출고일시" }, rows, 20);
}
