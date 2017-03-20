#include "message_converter.h"

using json = nlohmann::json;

MessageConverter MessageConverter::instance{};

MessageConverter& MessageConverter::getInstance() {
  return instance;
}

std::string MessageConverter::convertDeviceDataToJson(device_data data)
{
    json tmp = {
        {"id", data.id},
        {"type", data.type},
        {"value", data.data},
        {"category", data.category}
    };

    return tmp.dump();
}

device_command MessageConverter::convertJsonToDeviceCommand(std::string command) {
  json tmp = json::parse(command);

  device_command result{tmp["id"], tmp["targetState"]};

  return result;
}

