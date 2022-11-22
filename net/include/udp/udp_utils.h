#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "net.h"

namespace lm {
    namespace spp {

        class UdpUtilsSync {

        public:
            UdpUtilsSync();
            void Send(std::string host, unsigned short port,const std::string &data);
            std::string Receive(std::string host, unsigned short port);
            virtual ~UdpUtilsSync();

        protected:
            // This is the object providing access to the operating system's communication services
            // used by the socket object
            boost::asio::io_service m_ios;
            // UDP socket used for communication
            boost::asio::ip::udp::socket m_sock;
        };


    }
}

