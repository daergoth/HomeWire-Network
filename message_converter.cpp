#include "message_converter.h"

using json = nlohmann::json;


std::string MessageConverter::convertSensorData(sensor_data data)
{
    json tmp = {
        {"id", data.id},
        {"type", data.type},
        {"value", data.data}
    };
    
    
    return tmp.dump();
}

