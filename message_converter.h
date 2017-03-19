#ifndef MESSAGE_CONVERTER_H
#define MESSAGE_CONVERTER_H

#include "mesh_handler.h"
#include "json.hpp"

class MessageConverter {
public:
    static MessageConverter& getInstance();

    std::string convertDeviceDataToJson(device_data data);

    device_command convertJsonToDeviceCommand(std::string command);
private:
    static MessageConverter instance;

    MessageConverter() = default;

};

#endif
