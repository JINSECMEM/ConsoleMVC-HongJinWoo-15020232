#include "ProductionController.h"
#include "../Views/ConsoleHelper.h"

ProductionController::ProductionController(ProductionService& prodSvc, ProductionView& view)
    : prodSvc_(prodSvc), view_(view) {}

void ProductionController::ShowLines() {
    view_.RenderLines(prodSvc_.GetAllLines());
    view_.RenderJobs(prodSvc_.GetAllJobs());
    ConsoleHelper::Pause();
}

void ProductionController::ShowJobs() {
    view_.RenderJobs(prodSvc_.GetAllJobs());
    ConsoleHelper::Pause();
}
