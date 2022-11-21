#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include "net.h"


namespace lm {
    namespace spp {

        class UdpUtilsSync {
            ATTR(boost::asio::io_service, m_ios, IO)
            ATTR(boost::asio::ip::tcp::endpoint, m_ep, Endpoint)
            ATTR(boost::asio::ip::tcp::socket, m_sock, Socket)
        public:
            UdpUtilsSync(const std::string &host, unsigned short port_num);
            void Send(const std::string &data);
            std::string Receive();
            virtual ~UdpUtilsSync();
        };

// TODO: Explore Template Yet possible always byte array so ...
/*
class UdpUtilsSync2 {
public:
    UdpUtilsSync2(const std::string& host, unsigned short port_num);

    void Send(const  uint8_t * data);

    uint8_t*  Receive();

    virtual ~UdpUtilsSync();

private:
    boost::asio::io_service m_ios;
    boost::asio::ip::tcp::endpoint m_ep;
    boost::asio::ip::tcp::socket m_sock;
};

template<typename T>
class UdpUtilsSync3 {
public:
    UdpUtilsSync2(const std::string& host, unsigned short port_num);

    void Send(const  T data);

    T  Receive();

    virtual ~UdpUtilsSync();

private:
    boost::asio::io_service m_ios;
    boost::asio::ip::tcp::endpoint m_ep;
    boost::asio::ip::tcp::socket m_sock;
};
*/

    }
}

