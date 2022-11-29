#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "net.h"

namespace lm {
    namespace spp {


        template<class Req, std::size_t N = sizeof(Req)>
        void UdpSend(std::string host, unsigned short port, Req req ) {
            boost::asio::io_service ios;
            boost::asio::ip::udp::socket udp_socket(ios);
            udp_socket.open(asio::ip::udp::v4());
            auto udp_ep = utils::GetUdpEndpoint(host, port);
            
            boost::array<char, N> data;
            char* p = lm::spp::Serialize(req);
            int index = 0;
            for (char c : data) {
                data[index] = *(p + index);
            }
            auto bufferedData = asio::buffer(data);

            udp_socket.send_to(bufferedData, udp_ep);

            udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            udp_socket.close();
        }

        template<class Req, class Res, std::size_t N = sizeof(Req), std::size_t M= sizeof(Res)>
        std::shared_ptr<Res*> UdpRequestReply(std::string host, unsigned short port, Req req) {
            boost::asio::io_service ios;
            UdpSend<Req>(host,port,req);
 
            boost::asio::ip::udp::socket udp_socket(ios);          
            udp_socket.open(asio::ip::udp::v4());
            boost::asio::ip::udp::endpoint sender_endpoint;

            char response[M];
            size_t bytes_received = udp_socket.receive_from(boost::asio::buffer(response), sender_endpoint);
 
            Res* res = lm::spp::DeSerialize<Res>(response);

            udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            udp_socket.close();

            return std::make_shared <Res*>(res);
        }
 
        template<class Req , std::size_t N = sizeof(Req) >
        void UpdConsumeOne(std::string host, unsigned short port, std::function<void(std::shared_ptr<Req>)> handler) {
            boost::asio::io_service ios;
            using namespace  boost::asio::ip::udp;
            socket socket(io_context, endpoint( boost::asio::ip::udp::v4(), port));

            char request[N];
            endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(request), remote_endpoint);

            auto res = lm::spp::DeSerialize<Req>(response);
            auto res = std::make_shared <Res*>(res);
            handler(res);
        }


        typedef char byte;
 
        class UdpUtilsSync {
        public:
            UdpUtilsSync();
            void SendTo(std::string host, unsigned short port,const char* pdata, size_t len);
            boost::array<char, 1024> ServerReceiveNoReply(std::string host, unsigned short port);
            boost::array<char, 1024> BlockingRead();
            boost::array<char, 1024> RequestReply(std::string host, unsigned short port, char* pdata, size_t len);
            void ReceiveReply(std::string host, unsigned short port , std::function<boost::array<char, 1024>(boost::array<char,1024>)> handler);
 /*            
            boost::array<char, N> ClientReceive(std::string host, unsigned short port); 
*/            
            virtual ~UdpUtilsSync();
        protected:
            boost::asio::io_service m_ios;
            boost::asio::ip::udp::socket m_udp_socket;
        };
    }
}

