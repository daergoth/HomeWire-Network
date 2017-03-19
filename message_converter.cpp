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
  std::cerr << "MessageConverter.convertJsonToDeviceCommand(): " << command << std::endl;

  json tmp = json::parse(command);

  std::cerr <<  "tmp: " << tmp << std::endl;
  std::cerr <<  "tmp[\"id\"]: " << tmp["id"] << std::endl;

  device_command result{tmp["id"], tmp["targetState"]};

  std::cerr << "result: id:" << result.id << ", targetState:" << result.targetState << std::endl;

  return result;
}

