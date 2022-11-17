#include <iostream>

#include <udp_server.h>
#include <boost/asio.hpp>
#include <string>

int main() {
    int ret = 0;
    try {
        bool isRunning = true;
        boost::asio::io_context ioContext;
        auto ipv4 = boost::asio::ip::tcp::v4();
        unsigned short port = 7767;
        boost::asio::ip::tcp::endpoint ep(ipv4, port);
        boost::asio::ip::tcp::acceptor acceptor(ioContext, ep);

        while (isRunning) {
            boost::asio::ip::tcp::socket socket(ioContext);
            acceptor.accept(socket);
            std::string msg = "Message \n ";
            boost::system::error_code e;
            boost::asio::write(socket, boost::asio::buffer(msg), e);

        }
    }catch(std::exception& x ){
        std::cerr << x.what() << std::endl;
        ret = -1;
    }
    return ret;
}
