#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include <string>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class SocketHandler {
public:
    SocketHandler(std::string hostname, std::string port_num);
    
    void connect();
    
    bool sendString(std::string message);
    
private:
    boost::asio::io_service io_service;
    tcp::resolver resolver;
    tcp::resolver::query query;
    
    tcp::socket socket;
};

#endif
