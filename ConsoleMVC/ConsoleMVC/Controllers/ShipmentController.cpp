#include "ShipmentController.h"
#include "../Views/ConsoleHelper.h"

ShipmentController::ShipmentController(ShipmentService& shipSvc, ShipmentView& view)
    : shipSvc_(shipSvc), view_(view) {}

void ShipmentController::ShowHistory() {
    view_.RenderHistory(shipSvc_.GetHistory());
    ConsoleHelper::Pause();
}
