#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include <string>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "mesh_handler.h"

using boost::asio::ip::tcp;

class SocketHandler {
public:
    static SocketHandler& getInstance();

    boost::thread startListening();

    void connect();

    void sendString(std::string message);

private:
    static SocketHandler instance;

    std::vector<std::string> json_buffer;
    std::mutex json_buffer_mutex;

    boost::asio::io_service io_service;

    tcp::resolver resolver;
    tcp::resolver::query query;
    tcp::socket socket;

    boost::asio::streambuf tcp_buffer;

    SocketHandler(std::string hostname, std::string port_num)
        :io_service(), resolver(io_service), query(tcp::v4(), hostname, port_num), socket(io_service) {}

    void loop();
};

#endif
