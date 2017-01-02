#include <iostream>
#include "mesh_handler.h"
#include "socket_handler.h"
#include "message_converter.h"
#include "stdio.h"
#include "printf.h"

SocketHandler socket_handler("192.168.0.102", "45678");
 
MeshHandler mesh_handler = MeshHandler();

MessageConverter message_converter = MessageConverter();

std::vector<sensor_data> sensorBuffer;

void setup() {
    std::cerr << "Start" << std::endl;
    
    socket_handler.connect();
    
    mesh_handler.setupMesh();
}

void loop() {
    mesh_handler.updateMesh(); 
    
    int dataCount = mesh_handler.readAvailableData(sensorBuffer);
    
    for (int i = 0; i < dataCount; ++i) {
        if (!socket_handler.sendString(message_converter.convertSensorData(sensorBuffer[i]))) {
			socket_handler.connect();
			break;
		}
    }
    
    sensorBuffer.clear();
}

int main() {
  setup();
    
  while(1) {
    loop();
    
    delay(2);
  }
    
  return 0;
}

