#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>
#include <boost/array.hpp>

#include <net.h>

#include <boost/circular_buffer.hpp>

using namespace std;
using namespace lm::spp;

int main() {

    bool isRunning = true;
    std::string host = "127.0.0.1";
    unsigned short port = 7767;


    try {
        // client sending thread
        int send_count = 0;
        std::thread t([&]() {
            UdpUtilsSync udpUtil;
            while (isRunning) {
                stringstream ss;
                ss << "Sending Some Data " << send_count << endl;
                string data = ss.str();
                try {
                    udpUtil.ClientSendAndForget(host, port, data);
                    send_count++;
                    std::this_thread::sleep_for(1000ms);
                }
                catch (std::exception &ex) {
                    std::cerr << ex.what() << endl;
                    throw ex;
                }
            }
            std::cout << "Sending Thread exiting" << std::endl;
        });




        // receiving thread, we are acting like a server
        boost::circular_buffer<std::string> circular_buffer(3);

        // Works
        //        std::thread q([&]() {
        //            boost::asio::io_service io_context;
        //            boost::asio::ip::udp::socket socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 7767));
        //            while (isRunning) {
        //                boost::array<char, 24> recv_buf;
        //                boost::asio::ip::udp::endpoint remote_endpoint;
        //                std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
        //                string data(recv_buf.begin(), bytes_received);
        //                circular_buffer.push_back(data);
        //            }
        //        });


//        auto server = [&] (){
        std::thread q([&]() {
            UdpUtilsSync udpUtil;
            while (isRunning) {
                try {
                    auto data = udpUtil.ServerReceiveAndForget(host, port);
                    circular_buffer.push_back(data);
                }
                catch (std::exception &ex) {
                    std::cerr << ex.what() << endl;
                    throw ex;
                }
            }
            std::cout << "Receiving Thread exiting" << std::endl;
        });
//        };
//        server();



        std::cout << "Main thread pause for 30s" << std::endl;
        std::this_thread::sleep_for(30 * 1000ms);
        isRunning = false;
        std::this_thread::sleep_for(1000ms);

        // take look at the circular buffer

        int index = 0;
        for_each(begin(circular_buffer), end(circular_buffer), [&](std::string m) {
            std::cout << "index:" << index << "data:" << m << std::endl;
            index ++;
        });

        std::cout << "Main thread falling off the end of it brace" << std::endl;

    }
    catch (...) {
        std::cout << "Unexpected error" << std::endl;
        return -9;
    }
    return 0;
}
