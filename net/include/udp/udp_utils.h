#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "net.h"

namespace lm {
    namespace spp {

        //template <class R, class R>
        //class UdpService {
        //public:
        //    UdpService(std::string host, unsigned short port):m_host(host),m_port(port), m_udp_socket(m_ios) {

        //    }

        //    virtual void Send(R data, std::function<void(R)> completionHandler) {
        //        auto udp_ep = utils::GetUdpEndpoint(host, port);
        //        auto bufferedData = asio::buffer(data);
        //        m_udp_socket.send_to(bufferedData, udp_ep);
        //        completionHandler(data);
        //    }


        //    virtual void OnConnect(std::function<void(boost::asio::ip::udp::endpoint)> handler) = 0;
        //    
        //    virtual void Listening() {
        //        char response[1024];
        //        boost::asio::ip::udp::endpoint sender_endpoint;
        //        size_t bytes_received = m_udp_socket.receive_from(boost::asio::buffer(response), sender_endpoint);
        //        std::string rcvData(response, bytes_received);
        //        OnMessage();
        //    }

        //    // The 'message' event is emitted when a new datagram is available on a socket. 
        //    // The event handler function is passed two arguments: 
        //    // msg and rinfo.
        //    virtual void OnMessage(R data, (boost::asio::ip::udp::endpoint epRemote, std::function<void(R)> handler) = 0;
        //    virtual void OnError() = 0;

        //protected:
        //    boost::asio::io_service m_ios;
        //    std::string m_host;
        //    unsigned short m_port;
        //    boost::asio::ip::udp::socket m_udp_socket;
        //};

        class UdpUtilsSync {

        public:
            UdpUtilsSync();

            void SendTo(std::string host, unsigned short port, const std::string& data);

            std::string ListenAndReply();

            void RequestAndForget(std::string host, unsigned short port, const std::string &data);

            std::string RequestReply(std::string host, unsigned short port, const std::string& data);
            
            std::string ClientReceive(std::string host, unsigned short port);

            std::string ReceiveNoReply(std::string host, unsigned short port);

            std::string ReceiveAndReply(std::string host, unsigned short port);

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

