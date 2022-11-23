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
        auto client = [&]() {
            int send_count = 0;
            auto t = new std::thread([&]() {
                UdpUtilsSync udpUtil;
                while (isRunning) {
                    stringstream ss;
                    ss << "Sending Some Data " << send_count << endl;
                    string data = ss.str();
                    try {
                        udpUtil.ClientSendAndForget(host, port, data);
                        send_count++;
                        cout << send_count << ": ClientSendAndForget:" << data << endl;
                        std::this_thread::sleep_for(1000ms);
                    }
                    catch (std::exception &ex) {
                        std::cerr << ex.what() << endl;
                        throw ex;
                    }
                }
                std::cout << "Sending Thread exiting" << std::endl;
            });
            return t;
        };
        client()->detach();


        // receiving thread, we are acting like a server
        boost::circular_buffer<std::string> circular_buffer(3);

        auto server = [&]() {
            auto t = new std::thread([&]() {
                UdpUtilsSync udpUtil;
                int recv_count = 0;
                while (isRunning) {
                    try {
                        auto data = udpUtil.ServerReceiveAndForget(host, port);
                        cout << recv_count << ": ServerReceiveAndForget:" << data << endl;
                        recv_count++;
                        circular_buffer.push_back(data);
                    }
                    catch (std::exception &ex) {
                        std::cerr << ex.what() << endl;
                        throw ex;
                    }
                }
                std::cout << "Receiving Thread exiting" << std::endl;
            });
            return t;
        };
        server()->detach();


        std::cout << "Main thread pause for 30s" << std::endl;
        std::this_thread::sleep_for(30 * 1000ms);
        isRunning = false;
        std::this_thread::sleep_for(2000ms);

        // take look at the circular buffer

        int index = 0;
        for_each(begin(circular_buffer), end(circular_buffer), [&](std::string m) {
            std::cout << "index:" << index << "data:" << m << std::endl;
            index++;
        });


        std::cout << "Main thread falling off the end of it brace" << std::endl;

    }
    catch (...) {
        std::cout << "Unexpected error" << std::endl;
        return -9;
    }
    return 0;
}
