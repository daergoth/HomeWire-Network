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
  std::cerr << "MessageConverter.convertJsonToActorCommand(): " << command << std::endl;

  json tmp = json::parse(command);

  std::cerr <<  "tmp: " << tmp << std::endl;
  std::cerr <<  "tmp[\"id\"]: " << tmp["id"] << std::endl;

  actor_command result{tmp["id"], tmp["targetState"]};

  std::cerr << "result: id:" << result.id << ", targetState:" << result.targetState << std::endl;

  return result;
}

