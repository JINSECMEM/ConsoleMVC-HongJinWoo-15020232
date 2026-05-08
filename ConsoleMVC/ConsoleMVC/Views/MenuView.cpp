#include "MenuView.h"
#include "ConsoleHelper.h"
#include <iostream>

void MenuView::Render() {
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_CYAN);
    std::cout << "============================================================\n";
    std::cout << "       반도체 시료 생산 관리 시스템  v1.0  (PoC)\n";
    std::cout << "============================================================\n";
    ConsoleHelper::ResetColor();
    std::cout << "  [1] 주문 등록\n"
              << "  [2] 주문 목록 조회\n"
              << "  [3] 주문 취소\n"
              << "  [4] 재고 현황 조회\n"
              << "  [5] 생산라인 현황 조회\n"
              << "  [6] 출고 이력 조회\n"
              << "  [7] 모니터링 대시보드 (실시간)\n"
              << "  [0] 종료\n";
    ConsoleHelper::PrintDivider();
    std::cout << "  선택 > ";
}

int MenuView::GetChoice() {
    int choice = -1;
    std::cin >> choice;
    std::cin.ignore(1000, '\n');
    return choice;
}
