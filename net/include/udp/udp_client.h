#pragma once

#include <iostream>
#include <boost/asio.hpp>


class UdpClientSync {
public:
    UdpClientSync(const std::string &raw_ip_address, unsigned short port_num);

    virtual ~UdpClientSync();

    void fireAndForget(const std::string &request);

private:
    boost::asio::io_service m_ios;
    boost::asio::ip::tcp::endpoint m_ep;
    boost::asio::ip::tcp::socket m_sock;
};




