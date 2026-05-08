#include "ProductionView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <iomanip>

void ProductionView::RenderLines(const std::vector<ProductionLine>& lines) {
    ConsoleHelper::PrintDivider('=');
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_YELLOW);
    std::cout << "  생산라인 현황\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider();

    for (auto& l : lines) {
        bool running = l.status == LineStatus::RUNNING;
        std::cout << "  " << std::left << std::setw(8) << l.lineId
                  << std::setw(14) << LineStatusStr(l.status);
        if (running) {
            ConsoleHelper::SetColor(ConsoleHelper::COLOR_GREEN);
            ConsoleHelper::PrintProgressBar(l.progressPct);
            std::cout << "  Job: " << l.currentJobId;
            ConsoleHelper::ResetColor();
        } else {
            std::cout << "-";
        }
        std::cout << '\n';
    }
}

void ProductionView::RenderJobs(const std::vector<ProductionJob>& jobs) {
    ConsoleHelper::PrintDivider();
    ConsoleHelper::SetColor(ConsoleHelper::COLOR_YELLOW);
    std::cout << "  생산 작업 이력\n";
    ConsoleHelper::ResetColor();
    ConsoleHelper::PrintDivider();

    if (jobs.empty()) { std::cout << "  (작업 없음)\n"; return; }

    std::vector<std::vector<std::string>> rows;
    for (auto& j : jobs) {
        const char* s = j.status == JobStatus::COMPLETED ? "COMPLETED" :
                        j.status == JobStatus::RUNNING   ? "RUNNING"   :
                        j.status == JobStatus::QUEUED    ? "QUEUED"    : "FAILED";
        rows.push_back({ j.jobId, j.sampleId, j.lineId,
                         std::to_string(j.targetQty), s });
    }
    ConsoleHelper::PrintTable({ "작업ID", "시료", "라인", "목표수량", "상태" }, rows, 18);
}
