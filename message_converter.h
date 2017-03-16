#ifndef MESSAGE_CONVERTER_H
#define MESSAGE_CONVERTER_H

#include "mesh_handler.h"
#include "json.hpp"

class MessageConverter {
public:
    static MessageConverter& getInstance();

    std::string convertSensorDataToJson(sensor_data data);

    actor_command convertJsonToActorCommand(std::string command);
private:
    static MessageConverter instance;

    MessageConverter() = default;

};

#endif
