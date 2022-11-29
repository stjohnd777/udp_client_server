#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <functional>
#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "net.h"

namespace lm {
    namespace spp {

        typedef
        std::function <
                std::tuple<size_t,std::shared_ptr<char[]>> (std::tuple<size_t,std::shared_ptr<char[]>>)
        > REQ_REPLY_HANDLER;

        template<class Req, std::size_t N = sizeof(Req)>
        void UdpSend(std::string host, unsigned short port, Req req ) {

            boost::array<char, N> data;
            char* p = lm::spp::Serialize(req);
            int index = 0;
            for (char c : data) {
                data[index] = *(p + index);
            }
            auto bufferedData = asio::buffer(data);

            boost::asio::io_service ios;
            boost::asio::ip::udp::socket udp_socket(ios);
            udp_socket.open(asio::ip::udp::v4());
            auto udp_ep = utils::GetUdpEndpoint(host, port);
            udp_socket.send_to(bufferedData, udp_ep);

            udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            udp_socket.close();
        }

        template<class Req, class Res, std::size_t N = sizeof(Req), std::size_t M= sizeof(Res)>
        std::shared_ptr<Res*> UdpRequestReply(std::string host, unsigned short port, Req req) {
            boost::asio::io_service ios;
            UdpSend<Req>(host,port,req);


            char response[M];
            boost::asio::ip::udp::socket udp_socket(ios);
            udp_socket.open(asio::ip::udp::v4());
            boost::asio::ip::udp::endpoint sender_endpoint;
            size_t bytes_received = udp_socket.receive_from(boost::asio::buffer(response), sender_endpoint);
 
            Res* res = lm::spp::DeSerialize<Res>(response);

            udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            udp_socket.close();

            return std::make_shared <Res*>(res);
        }
 
        template<class Req , std::size_t N = sizeof(Req) >
        void UpdConsumeOne(std::string host, unsigned short port, std::function<void(std::shared_ptr<Req>)> handler) {

            boost::asio::io_service ios;
            boost::asio::ip::udp::socket socket(ios, boost::asio::ip::udp::endpoint( boost::asio::ip::udp::v4(), port));

            char request[N];
            boost::asio::ip::udp::endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(request), remote_endpoint);

            auto res = lm::spp::DeSerialize<Req>(request);
            handler(res);

            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            socket.close();
        }


        typedef char byte;
 
        class UdpUtilsSync {
        public:
            UdpUtilsSync();
            void SendTo(std::string host, unsigned short port,const char* pdata, size_t len);

            std::tuple<size_t,std::shared_ptr<char[]> >
            ServerReceiveNoReply(std::string host, unsigned short port);

            std::tuple<size_t,std::shared_ptr<char[]> >
            BlockingRead();

            std::tuple<size_t,std::shared_ptr<char[]>>
            RequestReply(std::string host, unsigned short port, const char* pdata, size_t len);

            void ReceiveReply(std::string host, unsigned short port ,  REQ_REPLY_HANDLER handler);
            virtual ~UdpUtilsSync();
        protected:
            boost::asio::io_service m_ios;
            boost::asio::ip::udp::socket m_udp_socket;
        };
    }
}

