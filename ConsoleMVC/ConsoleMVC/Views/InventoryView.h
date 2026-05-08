#pragma once
#include "../Models/Inventory.h"
#include <vector>

class InventoryView {
public:
    void RenderInventoryTable(const std::vector<Inventory>& inventories);
};
