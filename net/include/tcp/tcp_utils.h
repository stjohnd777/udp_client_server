//
// Created by Daniel St. John on 11/17/22.
//

#pragma once

#include "net.h"
#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

namespace lm {
    namespace spp {

        class TCPUtils {
            ATTR(boost::asio::io_service, IO)
            ATTR(tcp::socket *, m_socket, Socret)
        public:
            TCPUtils();
            string read();
            void send(const string &message);
            virtual ~TCPUtils();
        };
    }
}


