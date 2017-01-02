#include "socket_handler.h"

SocketHandler::SocketHandler(std::string hostname, std::string port_num)
    :io_service(), resolver(io_service), query(hostname, port_num), socket(io_service)
{
}

void SocketHandler::connect()
{
    std::cerr << "Waiting for socket connection..." << std::endl;
    boost::asio::connect(socket, resolver.resolve(query));
    std::cerr << "Socket connected!" << std::endl;
}

bool SocketHandler::sendString(std::string message)
{
    //std::cerr << "Socket out: "<< message << std::endl;
    
    message.append("\n");
	
	try {
		socket.write_some(boost::asio::buffer(message));
	} catch (std::exception& e) {
		return false;
	}
    
    
    return true;
}
