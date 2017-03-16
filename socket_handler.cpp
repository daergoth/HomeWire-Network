#include "socket_handler.h"
#include "message_converter.h"

SocketHandler SocketHandler::instance{"192.168.0.102", "45678"};

SocketHandler& SocketHandler::getInstance() {
  return instance;
}

boost::thread SocketHandler::startListening() {
  std::cerr << "SocketHandler listening..." << std::endl;
  return boost::thread(boost::bind(&SocketHandler::loop, this));
}

void SocketHandler::connect()
{
    std::cerr << "Waiting for socket connection..." << std::endl;
    boost::asio::connect(socket, resolver.resolve(query));
    std::cerr << "Socket connected!" << std::endl;
}

bool SocketHandler::sendString(std::string message)
{
  message.append("\n");

	try {
		socket.write_some(boost::asio::buffer(message));
	} catch (std::exception& e) {
		return false;
	}

    return true;
}

void SocketHandler::loop() {
  while(1) {
    while(socket.available()) {
      char buffer[32];

      socket.read_some(boost::asio::buffer(buffer, 32));

      actor_command command = MessageConverter::getInstance().convertJsonToActorCommand(buffer);

      if(!MeshHandler::getInstance().writeToActor(command)) {
        std::cerr << "MeshHandler.writeToActor() Error: {id:" << command.id << ", targetState: "
                  << command.targetState << "}" << std::endl;
      }
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds{2});
  }
}
