#pragma once
#include "../Models/Shipment.h"
#include <vector>

class ShipmentView {
public:
    void RenderHistory(const std::vector<Shipment>& shipments);
};
