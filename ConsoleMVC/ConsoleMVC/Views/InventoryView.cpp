#include "InventoryView.h"
#include "ConsoleHelper.h"
#include <iostream>

void InventoryView::RenderInventoryTable(const std::vector<Inventory>& inventories) {
    ConsoleHelper::PrintDivider('=');
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_YELLOW);
    std::cout << "  재고 현황\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider();

    std::vector<std::vector<std::string>> rows;
    for (auto& inv : inventories) {
        bool low = inv.stock <= inv.threshold;
        std::string status = low ? "[!] LOW" : "OK";
        rows.push_back({ inv.sampleId,
                         std::to_string(inv.stock),
                         std::to_string(inv.threshold),
                         status });
    }

    ConsoleHelper::PrintTable({ "시료ID", "재고", "임계값", "상태" }, rows, 14);

    for (auto& inv : inventories) {
        if (inv.stock <= inv.threshold) {
            ConsoleHelper::SetColor(ConsoleHelper::COLOR_RED);
            std::cout << "  [경고] " << inv.sampleId
                      << " 재고가 임계값 이하입니다. (현재 "
                      << inv.stock << " / 임계값 " << inv.threshold << ")\n";
            ConsoleHelper::ResetColor();
        }
    }
}
