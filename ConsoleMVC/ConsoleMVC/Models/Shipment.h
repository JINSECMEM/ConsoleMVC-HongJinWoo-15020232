#pragma once
#include <string>

struct Shipment {
    std::string shipmentId;
    std::string orderId;
    std::string sampleId;
    int         quantity = 0;
    std::string shippedAt;
};
