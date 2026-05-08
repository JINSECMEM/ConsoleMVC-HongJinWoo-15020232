#include "MonitorView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

void MonitorView::Render(const std::vector<Order>&          orders,
                         const std::vector<Inventory>&      inventories,
                         const std::vector<ProductionLine>& lines,
                         const std::vector<std::string>&    eventLog,
                         int                                remainingSec) {
    ConsoleHelper::ClearScreen();
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_WHITE);
    std::cout << "==================== MONITORING DASHBOARD ====================\n";
    ConsoleHelper::ResetColor();

    RenderOrderSummary(orders);
    RenderInventory(inventories);
    RenderLines(lines);
    RenderEventLog(eventLog);

    ConsoleHelper::SetColor(ConsoleHelper::COLOR_CYAN);
    std::cout << "\n  [모니터링 중... " << remainingSec << "초 후 메뉴로 복귀]\n";
    ConsoleHelper::ResetColor();
    std::cout << "==============================================================\n";
}

void MonitorView::RenderOrderSummary(const std::vector<Order>& orders) {
    int pending = 0, inProd = 0, shipped = 0, cancelled = 0;
    for (auto& o : orders) {
        if (o.status == OrderStatus::PENDING || o.status == OrderStatus::CHECKING ||
            o.status == OrderStatus::READY_TO_SHIP || o.status == OrderStatus::AWAITING_PROD)
            pending++;
        else if (o.status == OrderStatus::IN_PRODUCTION) inProd++;
        else if (o.status == OrderStatus::SHIPPED)       shipped++;
        else if (o.status == OrderStatus::CANCELLED)     cancelled++;
    }
    std::cout << "\n  [주문 현황] 대기: " << pending
              << "  생산중: " << inProd
              << "  출고완료: " << shipped
              << "  취소: " << cancelled << "  (총 " << orders.size() << "건)\n";
}

void MonitorView::RenderInventory(const std::vector<Inventory>& inventories) {
    std::cout << "\n  [재고 현황]\n";
    std::cout << "  " << std::left << std::setw(10) << "시료ID"
              << std::setw(8) << "재고" << std::setw(8) << "임계값" << "상태\n";
    ConsoleHelper::PrintDivider('-', 40);
    for (auto& inv : inventories) {
        bool low = inv.stock <= inv.threshold;
        if (low) ConsoleHelper::SetColor(ConsoleHelper::COLOR_RED);
        std::cout << "  " << std::left << std::setw(10) << inv.sampleId
                  << std::setw(8) << inv.stock
                  << std::setw(8) << inv.threshold
                  << (low ? "[!] LOW" : "OK") << '\n';
        if (low) ConsoleHelper::ResetColor();
    }
}

void MonitorView::RenderLines(const std::vector<ProductionLine>& lines) {
    std::cout << "\n  [생산라인]\n";
    for (auto& l : lines) {
        std::cout << "  " << std::left << std::setw(8) << l.lineId
                  << std::setw(14) << LineStatusStr(l.status);
        if (l.status == LineStatus::RUNNING) {
            ConsoleHelper::SetColor(ConsoleHelper::COLOR_GREEN);
            ConsoleHelper::PrintProgressBar(l.progressPct, 15);
            ConsoleHelper::ResetColor();
        } else {
            std::cout << "  -";
        }
        std::cout << '\n';
    }
}

void MonitorView::RenderEventLog(const std::vector<std::string>& logs) {
    std::cout << "\n  [이벤트 로그]\n";
    ConsoleHelper::PrintDivider('-', 60);
    // 최근 10줄만 표시
    int start = std::max(0, static_cast<int>(logs.size()) - 10);
    for (int i = start; i < static_cast<int>(logs.size()); ++i)
        std::cout << "  " << logs[i] << '\n';
}
