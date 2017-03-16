#include "message_converter.h"

using json = nlohmann::json;

MessageConverter MessageConverter::instance{};

MessageConverter& MessageConverter::getInstance() {
  return instance;
}

std::string MessageConverter::convertSensorDataToJson(sensor_data data)
{
    json tmp = {
        {"id", data.id},
        {"type", data.type},
        {"value", data.data}
    };

    return tmp.dump();
}

actor_command MessageConverter::convertJsonToActorCommand(std::string command) {
  json tmp = command;

  return {tmp["targetState"]};
}

