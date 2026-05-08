#include "InventoryController.h"
#include "../Views/ConsoleHelper.h"

InventoryController::InventoryController(InventoryService& invSvc, InventoryView& view)
    : invSvc_(invSvc), view_(view) {}

void InventoryController::ShowInventory() {
    view_.RenderInventoryTable(invSvc_.GetAll());
    ConsoleHelper::Pause();
}
