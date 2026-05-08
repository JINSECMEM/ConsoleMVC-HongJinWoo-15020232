#include "AppController.h"
#include "../Views/ConsoleHelper.h"
#include "../Utils/Utils.h"
#include <iostream>
#include <thread>
#include <chrono>

AppController::AppController()
    // Services
    : orderSvc_(orderRepo_)
    , invSvc_(invRepo_)
    , prodSvc_(prodRepo_)
    , shipSvc_(shipRepo_, invSvc_, orderSvc_)
    // Controllers
    , orderCtrl_(orderSvc_, invSvc_, prodSvc_, shipSvc_, sampleRepo_, orderView_,
                 [this](const std::string& msg) { AddLog(msg); })
    , invCtrl_(invSvc_, invView_)
    , prodCtrl_(prodSvc_, prodView_)
    , shipCtrl_(shipSvc_, shipView_)
{
    // 생산 완료 콜백 등록
    prodSvc_.SetOnCompleteCallback([this](const ProductionJob& job) {
        invSvc_.Replenish(job.sampleId, job.completedQty);
        AddLog("생산 완료: " + job.jobId + "  +" + std::to_string(job.completedQty) + "개 입고");
        shipSvc_.RetryPending(job.orderId);
        AddLog("출고 처리: 주문 " + job.orderId);
    });

    AddLog("시스템 시작 — 초기 재고 로드 완료");
}

void AppController::Run() {
    while (running_) {
        ConsoleHelper::ClearScreen();
        menuView_.Render();
        int choice = menuView_.GetChoice();
        HandleMenu(choice);
    }
    std::cout << "\n  시스템을 종료합니다.\n";
}

void AppController::HandleMenu(int choice) {
    Tick(); // 메뉴 선택마다 생산 진행
    switch (choice) {
    case 1: orderCtrl_.RegisterOrder();  break;
    case 2: orderCtrl_.ListOrders();     break;
    case 3: orderCtrl_.CancelOrder();    break;
    case 4: invCtrl_.ShowInventory();    break;
    case 5: prodCtrl_.ShowLines();       break;
    case 6: shipCtrl_.ShowHistory();     break;
    case 7: ShowMonitor();               break;
    case 0: running_ = false;            break;
    default:
        std::cout << "\n  잘못된 선택입니다.\n";
        ConsoleHelper::Pause();
        break;
    }
}

void AppController::ShowMonitor() {
    constexpr int DURATION_SEC = 30;
    for (int i = DURATION_SEC; i > 0 && running_; --i) {
        Tick();
        monitorView_.Render(
            orderSvc_.GetAllOrders(),
            invSvc_.GetAll(),
            prodSvc_.GetAllLines(),
            eventLog_,
            i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void AppController::Tick() {
    prodSvc_.Tick();
}

void AppController::AddLog(const std::string& msg) {
    eventLog_.push_back("[" + GetTimestamp() + "] " + msg);
    // 최대 100줄 유지
    if (eventLog_.size() > 100)
        eventLog_.erase(eventLog_.begin());
}
