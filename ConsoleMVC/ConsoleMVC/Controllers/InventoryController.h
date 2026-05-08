#pragma once
#include "../Services/InventoryService.h"
#include "../Views/InventoryView.h"

class InventoryController {
public:
    InventoryController(InventoryService& invSvc, InventoryView& view);
    void ShowInventory();

private:
    InventoryService& invSvc_;
    InventoryView&    view_;
};
