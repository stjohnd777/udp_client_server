#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "net.h"

namespace lm {
    namespace spp {

        class UdpUtilsSync {

        public:
            UdpUtilsSync();
            void ClientSendAndForget(std::string host, unsigned short port, const std::string &data);
            std::string ClientReceive(std::string host, unsigned short port);

            std::string ServerReceiveAndForget(std::string host, unsigned short port){

                boost::asio::ip::udp::socket socket(m_ios, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 7767));

                boost::array<char, 24> recv_buf;
                boost::asio::ip::udp::endpoint remote_endpoint;
                std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
                std::string data(recv_buf.begin(), bytes_received);

//                std::string reply = "200:OK";
//                boost::system::error_code ignored_error;
//                socket.send_to(boost::asio::buffer(reply),remote_endpoint, 0, ignored_error);

                return data;
            }

            std::string ServerReceiveAndReply(std::string host, unsigned short port ){
                boost::asio::ip::udp::socket socket(m_ios, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 7767));
                boost::array<char, 24> recv_buf;
                boost::asio::ip::udp::endpoint remote_endpoint;
                std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
                std::string data(recv_buf.begin(), bytes_received);

                std::string reply = "200:OK";
                boost::system::error_code ignored_error;
                socket.send_to(boost::asio::buffer(reply),remote_endpoint, 0, ignored_error);

                return data;
            }
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

