#include "OrderView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <iomanip>

OrderInputDto OrderView::GetOrderInput(const std::vector<Sample>& samples) {
    ConsoleHelper::PrintDivider('=');
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_YELLOW);
    std::cout << "  주문 등록\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider();

    OrderInputDto dto;
    std::cout << "  고객 ID : "; std::getline(std::cin, dto.customerId);

    std::cout << "  시료 선택:\n";
    for (int i = 0; i < static_cast<int>(samples.size()); ++i)
        std::cout << "    [" << (i + 1) << "] " << samples[i].sampleId
                  << "  " << samples[i].name << '\n';
    std::cout << "  번호 선택 > ";
    int sel; std::cin >> sel; std::cin.ignore(1000, '\n');
    if (sel >= 1 && sel <= static_cast<int>(samples.size()))
        dto.sampleId = samples[sel - 1].sampleId;

    std::cout << "  주문 수량 : "; std::cin >> dto.quantity; std::cin.ignore(1000, '\n');
    std::cout << "  납기일 (YYYY-MM-DD) : "; std::getline(std::cin, dto.dueDate);
    return dto;
}

std::string OrderView::GetOrderIdInput() {
    std::cout << "  취소할 주문 ID : ";
    std::string id; std::getline(std::cin, id);
    return id;
}

void OrderView::RenderOrderList(const std::vector<Order>& orders) {
    ConsoleHelper::PrintDivider('=');
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_YELLOW);
    std::cout << "  주문 목록 (" << orders.size() << "건)\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider();

    if (orders.empty()) { std::cout << "  (주문 없음)\n"; return; }

    ConsoleHelper::PrintTable(
        { "주문ID", "고객ID", "시료", "수량", "납기일", "상태" },
        [&] {
            std::vector<std::vector<std::string>> rows;
            for (auto& o : orders)
                rows.push_back({ o.orderId, o.customerId, o.sampleId,
                                 std::to_string(o.quantity), o.dueDate,
                                 OrderStatusStr(o.status) });
            return rows;
        }(), 14);
}

void OrderView::RenderOrderResult(const Order& order, bool shipped, const std::string& msg) {
    ConsoleHelper::PrintDivider();
    ConsoleHelper::SetColor(shipped ? ConsoleHelper::COLOR_GREEN : ConsoleHelper::COLOR_YELLOW);
    std::cout << "  [결과] " << msg << '\n';
    ConsoleHelper::ResetColor();
    std::cout << "  주문 ID : " << order.orderId << '\n'
              << "  시료    : " << order.sampleId << '\n'
              << "  수량    : " << order.quantity << "개\n"
              << "  상태    : " << OrderStatusStr(order.status) << '\n';
}
