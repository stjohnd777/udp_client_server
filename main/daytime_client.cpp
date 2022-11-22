//
// Created by Owner on 11/21/2022.
//
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;


int main(int argc, char* argv[])
{
    try
    {
        string host = "127.0.0.1";

        boost::asio::io_context io_context;

        udp::endpoint receiver_endpoint = udp::endpoint(boost::asio::ip::address::from_string(host),7767);

        udp::socket socket(io_context);
        socket.open(udp::v4());

        boost::array<char, 1> send_buf  = {{ 0 }};
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        boost::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout.write(recv_buf.data(), len);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}