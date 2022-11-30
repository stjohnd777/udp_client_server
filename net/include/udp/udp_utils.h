#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <functional>
#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "net.h"


// The correct maximum UDP message size is 65507, 
// as determined by the following formula : 
// 0xffff - (sizeof(IP Header) + sizeof(UDP Header)) = 65535 - (20 + 8) = 65507

#define MAX_DATAGRAM 65507

typedef char byte;

namespace lm {
    namespace spp {

        typedef std::function <std::tuple<size_t,std::shared_ptr<char[]>> (std::tuple<size_t,std::shared_ptr<char[]>>) > REQ_REPLY_HANDLER;

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
            boost::asio::ip::udp::socket socket(ios, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));

            char request[N];
            boost::asio::ip::udp::endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(request), remote_endpoint);

            auto res = lm::spp::DeSerialize<Req>(request);
            handler(res);

            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            socket.close();
        }

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

        template<class Req, class Reply >
        class UdpComsumer {
        public:
            std::shared_ptr<Req*> Receive(std::string host, unsigned short port, Req req) {
                auto data = udpUtil.ServerReceiveNoReply(host, port);
                Req* req = lm::spp::DeSerialize<Req>(data);
                return std::make_shared <Req*>(req);
            }

            void StartReceiving(std::string host, unsigned short port, std::function<void(Req*)> handler) {
                isRunning = true;
                while (isRunning) {
                    auto req = Receive(host, port, handler);
                    handler(req);
                }
            }

            void Stop() {
                isRunning false;
            }
        };

        template<class Req, class Res >
        class UdpSender {
        public:
            UdpSender(std::string host, unsigned short port) : m_host(host), m_port(port) {
            }
            void Send(Req& req) {
                lm::spp::UdpSend<Request>(m_host, m_port, req);
            }

        protected:
            std::string m_host;
            unsigned short m_port;
        };


        template<class Req, class Res >
        class UdpClient {
        public:
            UdpClient(std::string host, unsigned short port) : m_host(host), m_port(port) {
            }
            void Send(Req& req) {
                lm::spp::UdpSend<Request>(m_host, m_port, req);
            }

            std::shared_ptr<Res*> RequestReply(Req req) {
                return lm::spp::UdpRequestReply<Request, Response>(m_host, m_port, req);
            }

        protected:
            std::string m_host;
            unsigned short m_port;
        };

        template<class Req, class Reply >
        class UdpServer {
        public:
            std::shared_ptr<Reply*> ReceiveReply(std::string host, unsigned short port, std::function<Reply* (Req*)> handler) {
                char* data = udpUtil.RequestReply(host, port, data);
                Req* res = lm::spp::DeSerialize<Reply>(data);
                Reply* res = handler(res);
                return std::make_shared <Reply*>(res);
            }

            void StartReqRes(std::string host, unsigned short port, std::function<Reply* (Req*)> handler) {
                isRunning = true;
                while (isRunning) {
                    ReceiveReply(host, port, handler)
                }
            }
            void Stop() {
                isRunning false;
            }
        protected:
            lm::spp::UdpUtilsSync udpUtil;
            bool isRunning = false;
        };

    }
}

