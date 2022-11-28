#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "net.h"

namespace lm {
    namespace spp {

        typedef char byte;


        template<std::size_t N = 1024,std::size_t M = 1024>
        class UdpUtilsSync {

        public:
            UdpUtilsSync();

            void SendTo(std::string host, unsigned short port, const boost::array<char, N>& data);

            boost::array<char, N> BlockingRead();

            boost::array<char, M> RequestReply(std::string host, unsigned short port, const boost::array<char, N>& data);

            boost::array<char, N> ClientReceive(std::string host, unsigned short port);

            boost::array<char, N> ReceiveNoReply(std::string host, unsigned short port);

            void ReceiveAndReply(std::string host, unsigned short port , std::function<boost::array<char, M>(boost::array<char,N>)> handler);

            virtual ~UdpUtilsSync();

        protected:
            // This is the object providing access to the operating system's communication services
            // used by the socket object
            boost::asio::io_service m_ios;
            // UDP socket used for communication
            boost::asio::ip::udp::socket m_udp_socket;
        };


    }
}

