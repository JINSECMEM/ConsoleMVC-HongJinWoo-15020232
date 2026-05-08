#pragma once
#include "../Services/ProductionService.h"
#include "../Views/ProductionView.h"

class ProductionController {
public:
    ProductionController(ProductionService& prodSvc, ProductionView& view);
    void ShowLines();
    void ShowJobs();

private:
    ProductionService& prodSvc_;
    ProductionView&    view_;
};
