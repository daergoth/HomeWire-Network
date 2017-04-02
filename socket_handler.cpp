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

void SocketHandler::sendString(std::string message)
{
  json_buffer_mutex.lock();

  message.append("\n");
	json_buffer.push_back(message);

  json_buffer_mutex.unlock();
}

void SocketHandler::loop() {
  while(1) {
    if(socket.available()) {
      size_t readBytes = boost::asio::read_until(socket, tcp_buffer, "\r\n");

      std::string readLine;
      std::getline(std::istream(&tcp_buffer), readLine);

      if (readBytes) {
        device_command command;

        try {
          command = MessageConverter::getInstance().convertJsonToDeviceCommand(readLine);
        } catch (std::exception& e) {
          std::cerr << "Exception during json to command conversion: " << e.what() << std::endl;
          continue;
        }

        MeshHandler::getInstance().sendToDevice(command);
      }

    }

    if (json_buffer_mutex.try_lock()) {
      for (std::string json : json_buffer) {
        std::cerr << "JSON sending: \'" << json << "\' ...";

        try {
          socket.write_some(boost::asio::buffer(json));
          std::cerr << "success" << std::endl;
        } catch (std::exception& e) {
          std::cerr << "FAIL" << std::endl;
          connect();
        }
      }
      json_buffer.clear();
      json_buffer_mutex.unlock();
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds{2});
  }
}
#pragma clang diagnostic pop
