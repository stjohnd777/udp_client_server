#pragma once

#include <string>
#include <iostream>
#include <functional>
#include <boost/asio.hpp>

using namespace boost;

class UdpServerSync {
public:
    UdpServerSync(const std::string &raw_ip_address, unsigned short port_num);

    virtual ~UdpServerSync();

    void start(std::function<void(std::string)>);

private:
    std::string receive();

private:
    bool isRunning = true;
    asio::ip::tcp::endpoint m_ep;
    asio::io_service m_ios;
    asio::ip::tcp::socket m_sock;
};

