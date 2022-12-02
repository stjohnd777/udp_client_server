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

        typedef std::function <std::tuple<size_t, std::shared_ptr<char[]>>(std::tuple<size_t, std::shared_ptr<char[]>>) > REQ_REPLY_HANDLER;

        /**
        This Class encapsulates UDP functionaliy for convienance
        
        The UDP server/service is on host:port the data is a byte array of lenght len.
        - SendTo // fire and forget
        - ServerReceiveNoReply  // comsumer
        - BlockingRead  // server listening for connection
        - RequestReply  // client request and reply 
        */
        class UdpUtilsSync {
        public:
            UdpUtilsSync();

            /**
            This method is mdoeling UDP send and foget 
            
            The UDP server/service is on host:port the data is a byte array of lenght len.
            @parm host
            @parm port
            @parm pdata
            @parm len
            @return void
            */
            void SendTo(std::string host, unsigned short port,const char* pdata, size_t len);

            /**
            This method is mdoeling UDP server receiving, acting as consumer
            
            @parm host
            @parm port
            @parm pdata
            @parm len
            @return  std::tuple<size_t,std::shared_ptr<char[]> >
            - <data length, bytes>
            */
            std::tuple<size_t,std::shared_ptr<char[]> >
            ServerReceiveNoReply(std::string host, unsigned short port);

           /**
            This method UDP server blocking reead
            
            @return  std::tuple<size_t,std::shared_ptr<char[]> >
            - <data length, bytes>
            */
            std::tuple<size_t,std::shared_ptr<char[]> >
            BlockingRead();

            /**
            This method is mdoeling UDP client request response behavior. 
            
            The UDP server/service is on host:port the data is a byte array of lenght len.
            @parm host
            @parm port
            @parm pdata
            @parm len
            */
            std::tuple<size_t,std::shared_ptr<char[]>>
            RequestReply(std::string host, unsigned short port, const char* pdata, size_t len);

            /**
            This method is mdoeling UDP server receive reply behavior.

            The UDP server/service is listening on host:port for connections
            @parm host
            @parm port
            @parm pdata
            @parm len
            */
            void ReceiveReply(std::string host, unsigned short port ,  REQ_REPLY_HANDLER handler);
            virtual ~UdpUtilsSync();

        protected:
            boost::asio::io_service m_ios;
            boost::asio::ip::udp::socket m_udp_socket;
        };

 



        // cleint send upd Req message does not expect response
        template<class Req>
        void UdpSend(std::string host, unsigned short port, Req req ) {

            auto p = lm::spp::Serialize(req);
            char send_buf[sizeof(Req)];
            memcpy(send_buf, p, sizeof(Req));
            auto bufferedData = asio::buffer(send_buf);

            // TODO: leverage 
             //boost::asio::io_service ios;
             //boost::asio::ip::udp::socket udp_socket(ios);
             //udp_socket.open(asio::ip::udp::v4());
             //auto udp_ep = utils::GetUdpEndpoint(host, port);
             //udp_socket.send_to(bufferedData, udp_ep);
            
             //udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
             //udp_socket.close();

            UdpUtilsSync udpClient;
            udpClient.SendTo(host,port,p,sizeof(req));

        }

        // server receives Req and process request with handler and does Reply to Sender
        template<class Req>
        void UpdConsumeOne(std::string host, unsigned short port, std::function<void(Req*)> handler) { // TODO:: maybe shared pointer Req

            // TODO : Dry
            boost::asio::io_service ios;
            auto udp_ep = utils::GetUdpEndpoint(host, port);
            boost::asio::ip::udp::socket socket(ios, udp_ep , port);

            char request[sizeof(Req)];
            boost::asio::ip::udp::endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(request), remote_endpoint);

            auto res = lm::spp::DeSerialize<Req>(request);
            handler(res);

            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            socket.close();

            // UdpClient udpClient;
            // std::tuple<size_t,std::shared_ptr<char[]> > tup =  udpClient.ServerReceiveNoReply(host,port)
            // auto bytesRes = get<1>(tup).get();
            // return  lm::spp::DeSerialize<Req>(bytesRes);
        }

        // client sends Req udp message and expects response Res
        template<class Req, class Res>
        Res* UdpRequestReply(std::string host, unsigned short port, Req req) {
            
            try {
                // send
                auto bytes_req = lm::spp::Serialize(req);
                char send_buf[sizeof(Req)];
                memcpy(send_buf, bytes_req, sizeof(Req));
                auto bufferedData = asio::buffer(send_buf);

                boost::asio::io_service ios;
                boost::asio::ip::udp::socket udp_socket(ios);
                udp_socket.open(asio::ip::udp::v4());
                auto udp_ep = utils::GetUdpEndpoint(host, port);
                udp_socket.send_to(bufferedData, udp_ep);


                char recv_buf[sizeof(Res)];
                boost::asio::ip::udp::endpoint remote_endpoint;
                std::size_t bytes_received = udp_socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
                Res* res = lm::spp::DeSerialize<Res>(recv_buf);
           
                // shutdown socket
                udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
                udp_socket.close();

                return res;
            }
            catch (boost::system::system_error& e) {
                cout << e.what() << endl;
                throw e;
            }
        }
 
        // server receives rand replys to sender with artifact procuded by handler
        template<class Req , class Res >
        void UpdRecieveReplyOne(std::string host, unsigned short port, std::function< Res*(Req*)> handler) {

            try {
                // initilize udp
                 boost::asio::io_service ios;
                 auto udp_ep = utils::GetUdpEndpoint(host, port);
                /*boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4()*/
                 boost::asio::ip::udp::socket socket(ios, udp_ep , port);

                // listening for incomming request from a sender
                char recv_buf[sizeof(Req)];
                boost::asio::ip::udp::endpoint senders_endpoint;
                std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), senders_endpoint);
                // assert bytes_received = M

                // deserialize client request
                auto req = lm::spp::DeSerialize<Req>(recv_buf);

                // the lambda computes response           
                auto res = handler(req);

                // serialize the response and place 
                auto size_bytes_res = sizeof(Res); // M
                auto bytes_res = Serialize(res);

                auto send_buf = new char[size_bytes_res];
                memcpy(send_buf, bytes_res, size_bytes_res);

                boost::system::error_code ignored_error;
                socket.send_to(boost::asio::buffer(send_buf, size_bytes_res), senders_endpoint, 0, ignored_error);

                // shutdown
                socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
                socket.close();
            }
            catch (boost::system::system_error& e) {
                cout << e.what() << endl;
                throw e;
            }
        }



        //////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //////////////////////////////////////////////////////////////////////////////////////////////////
        
        // Producer
        template<class Request, class Response >
        class UdpSender {
        public:
            UdpSender(std::string host, unsigned short port) : m_host(host), m_port(port) {
            }
            void Send(Request& req) {
                lm::spp::UdpSend<Request>(m_host, m_port, req);
            }

        protected:
            std::string m_host;
            unsigned short m_port;
        };

        // Consumer
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


        //////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //////////////////////////////////////////////////////////////////////////////////////////////////

        // Client
        template<class Request, class Response >
        class UdpClient {
        public:
            UdpClient(std::string host, unsigned short port) : m_host(host), m_port(port) {
            }
            void Send(Request& req) {
                lm::spp::UdpSend<Request>(m_host, m_port, req);
            }

            //std::shared_ptr<Response> RequestReply(Request req) {
            //    std::shared_ptr<Response> sp =  lm::spp::UdpRequestReply<Request, Response>(m_host, m_port, req);
            //    return sp;
            //}

            Response* RequestReply(Request req) {
               return lm::spp::UdpRequestReply<Request, Response>(m_host, m_port, req);
            }

        protected:
            std::string m_host;
            unsigned short m_port;
        };


        // Server
        template<class Request, class Response >
        class UdpServer {
        public:
            UdpServer(std::string host, unsigned short port) : m_host(host), m_port(port) {}

            void ReceiveReply( std::function<Response*(Request*)> handler) { 
                 UpdRecieveReplyOne<Request,Response>(m_host,m_port,handler);
            }

            void StartReqRes( std::function<Response* (Request*)> handler, bool isThread = false) {
               
                isRunning = true;

                auto worker = [&]() {
                    while (isRunning) {
                        ReceiveReply(handler);
                    }
                };

                if ( isThread) {
                    std::thread(worker);
                }else {
                  worker();
                }
                
            }
            void Stop() {
                isRunning =false;
            }
        protected:
            lm::spp::UdpUtilsSync udpUtil;
            bool isRunning = false;
            std::string m_host;
            unsigned short m_port;
        };

    }
}

