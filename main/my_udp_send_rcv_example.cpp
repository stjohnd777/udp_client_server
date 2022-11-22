#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>

#include <net.h>

#include <boost/circular_buffer.hpp>

using namespace std;
using namespace lm::spp;

int main() {

    bool isRunning = true;
    std::string host = "127.0.0.1";
    unsigned short port = 7767;
//    UdpUtilsSync udpUtil;
//    udpUtil.Send(host, port, "Test");
//    auto data = udpUtil.Receive(host, port);

    try {
        // sending thread, we are acting like a client
        auto client = [&]() {
            int send_count = 0;
            std::thread t([&]() {
                UdpUtilsSync udpUtil;
                while (isRunning) {
//                    stringstream ss;
//                    ss << "Sending Some Data " << send_count << endl;
//                    string data =  ss.str();
//                    std::string data = "Sending Some Data ";// + send_count ;
//                    char*  data = "Sending Some Data ";
                    try {
                        udpUtil.Send(host, port,"Some Data");
                        send_count++;
                        std::this_thread::sleep_for(1000ms);
                    }
                    catch (std::exception& ex) {
                        std::cerr << ex.what() << endl;
                        throw ex;
                    }
                }
                std::cout << "Sending Thread exiting" << std::endl;
                });
        };
        client();

        // receiving thread, we are acting like a server
        boost::circular_buffer<std::string> circular_buffer(3);
        auto server = [&]() {
            std::thread t([&]() {
                UdpUtilsSync udpUtil;
                while (isRunning) {
                    try {
                        auto data = udpUtil.Receive(host, port);
                        circular_buffer.push_back(data);
                    }
                    catch (std::exception& ex) {
                        std::cerr << ex.what() << endl;
                        throw ex;
                    }
                }
                std::cout << "Recieving Thread exiting" << std::endl;
                });
        };
        server();

        std::cout << "Main thread pause for 30s" << std::endl;
        std::this_thread::sleep_for(30 * 1000ms);
        isRunning = false;
        std::this_thread::sleep_for(1000ms);

        // take look at the circular buffer
        for_each(begin(circular_buffer), end(circular_buffer), [](std::string m) {
            static int index = 0;
            std::cout << "index:" << index << "data:" << m << std::endl;
            });

        std::cout << "Main thread falling off the end of it brace" << std::endl;
    }
    catch (...) {
        std::cout << "Unexpected error" << std::endl;
        return -9;
    }
    return 0;
}
