//
// Created by Daniel St. John on 11/17/22.
//

#include <"tcp/TCPUtils.h">

namespace lm {
    namespace spp {

        TCPUtils::TCPUtils() {
            //listen for new connection
            tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234));
            //socket creation
            m_socket = new tcp::socket(io_service);
            //waiting for connection
            acceptor_.accept(*m_socket);
        }

        string read() {
            boost::asio::streambuf buf;
            boost::asio::read_until(*m_socket, buf, "\n");
            string data = boost::asio::buffer_cast<const char *>(buf.data());
            return data;
        }

        void send(const string &message) {
            const string msg = message + "\n";
            boost::asio::write(*m_socket, boost::asio::buffer(message));
        }

        virtual ~

        TCPUtils() {
            delete m_socket;
        }

    }
}
