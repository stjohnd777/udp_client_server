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

void HaltMainForSec(unsigned short seconds) {
    std::cout << "Main thread pause for " << seconds <<  std::endl;
    std::this_thread::sleep_for(seconds * 1000ms);
    std::this_thread::sleep_for(1000ms);
}

int main() {

    bool isRunning = true;
    std::string host = "127.0.0.1";
    unsigned short port = 7767;

    try {

        auto startSending = [isRunning](string host, unsigned short port) {
            auto t = new thread([&](string host, unsigned short port) {
                UdpUtilsSync udpUtil;
                int send_count = 0;
                while (isRunning) {
                    stringstream ss;
                    ss << "data:" << send_count ;
                    string data = ss.str();
                    try {
                        size_t len =  data.length();
                        const char* pdata = data.c_str();
                        udpUtil.SendTo(host, port, pdata, len);
                        cout << "sending thread:" << std::this_thread::get_id()  << "sent:" << data << " len:" << len << endl;
                        send_count++;
                        std::this_thread::sleep_for(1000ms);
                    }
                    catch (std::exception& ex) {
                        std::cerr << ex.what() << endl;
                        throw ex;
                    }
                }
                std::cout << "thread:" << std::this_thread::get_id()  << " Sender Exiting" << std::endl;
                },host,port );
            return t;
        };
        startSending(host,port)->detach();
        startSending(host,port)->detach();
        startSending(host,port)->detach();



        // server
        auto startReceiving = [isRunning](string host, unsigned short port) {
            auto t = new thread([&](string host, unsigned short port) {

                boost::circular_buffer<std::string> circular_buffer(3);
                UdpUtilsSync udpUtil;
                while (isRunning) {
                    try {
                        auto t = udpUtil.ServerReceiveNoReply(host, port);
                        auto len = std::get<0>(t);
                        auto pChar = std::get<1>(t);
                        char s[MAX_DATAGRAM];
                        for ( size_t idx =0; idx < len; idx++){
                            char * p = pChar.get();
                            s[idx] = *( pChar.get() + idx);
                        }
                        s[len] = 0;
                        string str(s);
                        cout << "receiving thread:" << std::this_thread::get_id() <<  " Server Received " << str << endl;
                        circular_buffer.push_back(s);
                    }
                    catch (std::exception &ex) {
                        std::cerr << ex.what() << endl;
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
        startReceiving(host,port)->detach();


        HaltMainForSec(60);

        isRunning = false;

    }
    catch (...) {
        std::cout << "Unexpected error" << std::endl;
        return -9;
    }
    return 0;
}
