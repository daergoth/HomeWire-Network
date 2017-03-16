#include <iostream>
#include "mesh_handler.h"
#include "socket_handler.h"
#include "message_converter.h"
#include "RF24/printf.h"

SocketHandler& socket_handler = SocketHandler::getInstance();
 
MeshHandler& mesh_handler = MeshHandler::getInstance();

void setup() {
    std::cerr << "Start" << std::endl;
    
    socket_handler.connect();
    mesh_handler.setupMesh();
}

int main() {
  setup();

  boost::thread socketThread = socket_handler.startListening();
  boost::thread meshThread = mesh_handler.startListening();

  socketThread.join();
  meshThread.join();
    
  return 0;
}

