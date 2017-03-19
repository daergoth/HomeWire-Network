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
    if(socket.available()) {
      boost::this_thread::sleep_for(boost::chrono::milliseconds{10});

      char buff[32];
      size_t currB = 0;

      while(socket.available()) {
        currB += socket.read_some(boost::asio::buffer(buff + currB * sizeof(char), 31 - currB));
      }
      buff[currB] = 0;

      std::cerr << "read bytes: " << currB << std::endl;
      std::cerr <<  "buff: " << buff << std::endl;

      for (int i = 0; i < 32; ++i) {
        std::cerr << buff[i] << ". ";
      }
      std::cerr << std::endl;

      if (currB) {
        device_command command = MessageConverter::getInstance().convertJsonToDeviceCommand(buff);

        if(!MeshHandler::getInstance().sendToDevice(command)) {
          std::cerr << "MeshHandler.sendToDevice() Error: {id:" << command.id << ", targetState: "
                    << command.targetState << "}" << std::endl;
        } else {
          std::cerr << "write ok" << std::endl;
        }
      }


      memset(buff, 0, 32);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds{2});
  }
}
