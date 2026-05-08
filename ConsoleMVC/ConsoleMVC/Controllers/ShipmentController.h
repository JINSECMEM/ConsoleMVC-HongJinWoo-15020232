#pragma once
#include "../Services/ShipmentService.h"
#include "../Views/ShipmentView.h"

class ShipmentController {
public:
    ShipmentController(ShipmentService& shipSvc, ShipmentView& view);
    void ShowHistory();

private:
    ShipmentService& shipSvc_;
    ShipmentView&    view_;
};
