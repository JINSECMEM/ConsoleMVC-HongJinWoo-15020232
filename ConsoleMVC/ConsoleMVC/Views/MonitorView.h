#pragma once
#include "../Models/Order.h"
#include "../Models/Inventory.h"
#include "../Models/ProductionLine.h"
#include <vector>
#include <string>

class MonitorView {
public:
    void Render(const std::vector<Order>&         orders,
                const std::vector<Inventory>&     inventories,
                const std::vector<ProductionLine>& lines,
                const std::vector<std::string>&   eventLog,
                int                               remainingSec);
private:
    void RenderOrderSummary(const std::vector<Order>& orders);
    void RenderInventory(const std::vector<Inventory>& inventories);
    void RenderLines(const std::vector<ProductionLine>& lines);
    void RenderEventLog(const std::vector<std::string>& logs);
};
