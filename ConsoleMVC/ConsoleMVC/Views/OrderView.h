#pragma once
#include "../Models/Order.h"
#include "../Models/Sample.h"
#include <vector>
#include <string>

struct OrderInputDto {
    std::string customerId;
    std::string sampleId;
    int         quantity = 0;
    std::string dueDate;
};

class OrderView {
public:
    OrderInputDto      GetOrderInput(const std::vector<Sample>& samples);
    std::string        GetOrderIdInput();
    void               RenderOrderList(const std::vector<Order>& orders);
    void               RenderOrderResult(const Order& order, bool shipped, const std::string& msg);
};
