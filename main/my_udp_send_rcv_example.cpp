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

void HaltMainForSec(unsigned short seconds)
{
    std::cout << "Main thread pause for 30s" << std::endl;
    std::this_thread::sleep_for(seconds * 1000ms);
    std::this_thread::sleep_for(1000ms);
}
int main()
{

    bool isRunning = true;
    std::string host = "127.0.0.1";
    unsigned short port = 7767;

    try {
        // client
        auto client = [isRunning](string host, unsigned short port)
        {
            int send_count = 0;
            auto t = new thread([&](string host, unsigned short port) {
                UdpUtilsSync udpUtil;
                while (isRunning) {
                    stringstream ss;
                    ss << "Sending Some Data " << send_count << endl;
                    string data = ss.str();
                    try {
                        udpUtil.RequestAndForget(host, port, data);
                        cout << "client.RequestAndForget " << data << endl;
                        send_count++;
                        //std::this_thread::sleep_for(2000ms);
                        //auto reply = udpUtil.RequestReply(host, port, data);
                        //cout << "client.RequestReply " << data << ":" << reply << endl;
                    }
                    catch (std::exception& ex) {
                        std::cerr << ex.what() << endl;
                        throw ex;
                    }
                }
                std::cout << "Sending Thread exiting" << std::endl; 
                },host,port );
            return t;
        };
        client(host,port)->detach();

        // server
        auto server = [isRunning](string host, unsigned short port) {
            auto t = new thread([&](string host, unsigned short port) {
                boost::circular_buffer<std::string> circular_buffer(3);
                UdpUtilsSync udpUtil;
                while (isRunning) {
                    try {
                        auto data = udpUtil.ReceiveNoReply(host, port);
                        cout << "Server ReceiveNoReply " << data << endl;
                        circular_buffer.push_back(data);
                    }
                    catch (std::exception &ex) {
                        std::cerr << ex.what() << endl;
                        //throw ex;
                    }
                }
                std::cout << "Receiving Thread exiting" << std::endl;
                
                int index = 0;
                std::for_each(begin(circular_buffer), end(circular_buffer), [&](std::string m) {
                    std::cout << "index:" << index << "data:" << m << std::endl;
                    index++;
                    }); 
                },host,port);
            return t;
        };
        server(host,port)->detach();
      

        HaltMainForSec(30);
        isRunning = false;

    }
    catch (...) {
        std::cout << "Unexpected error" << std::endl;
        return -9;
    }
    return 0;
}
