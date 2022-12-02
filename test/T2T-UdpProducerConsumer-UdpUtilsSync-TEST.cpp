#define CATCH_CONFIG_MAIN // provides main(); this line is required in only one .cpp file
#include "catch.hpp"
#include <net.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

#include <boost/circular_buffer.hpp>

using namespace std;
using namespace lm::spp;
using namespace std::chrono;

void HaltMainForSec(unsigned short seconds) {
    std::cout << "Main thread pause for " << seconds <<  std::endl;
    std::this_thread::sleep_for(seconds * 1000ms);
    std::this_thread::sleep_for(1000ms);
}

TEST_CASE( "T2T-ProducerConsumer", "[1]" ) {
    REQUIRE( true == true );
    bool isRunning = true;
    std::string host = "127.0.0.1";
    unsigned short port = 7767;

        auto startSending = [isRunning](string host, unsigned short port) {
            auto t = new thread([&](string host, unsigned short port) {
                UdpUtilsSync udpUtil;
                int send_count = 0;
                while (isRunning) {
                    Request req;
                    req.seq = send_count;
                    req.gpsTime =  duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
                    req.cameraId = 1;
                    auto bytes = lm::spp::Serialize(req);
                    stringstream ss;
                    ss << "Sending Some Data " << bytes << endl;
                    udpUtil.SendTo(host, port,bytes,sizeof(Request));
                    cout << "sent:" << req.seq << ":" << req.gpsTime << ":" << req.cameraId << endl;

                    std::this_thread::sleep_for(1000ms);
                    send_count++;
   
                }
                std::cout << "thread:" << std::this_thread::get_id()  << " Sender Exiting" << std::endl;
                },host,port );
            return t;
        };
        startSending(host,port)->detach();
        startSending(host,port)->detach();
        startSending(host,port)->detach();

        auto startReceiving = [isRunning](string host, unsigned short port) {
            auto t = new thread([&](string host, unsigned short port) {

                boost::circular_buffer<Request*> circular_buffer(3);
                UdpUtilsSync udpUtil;
                while (isRunning) {
                    try {
                        auto t = udpUtil.ServerReceiveNoReply(host, port);
                        auto len = get<0>(t);
                        auto pChar = std::get<1>(t);
                        Request* req = lm::spp::DeSerialize<Request>(pChar.get());
                        cout << "receive:" << req->seq << ":" << req->gpsTime << ":" << req->cameraId << endl;
                        circular_buffer.push_back(req);
                    }
                    catch (std::exception &ex) {
                        std::cerr << ex.what() << endl;
                    }
                }
                std::cout << "Receiving Thread exiting" << std::endl;
                
                int index = 0;
                std::for_each(begin(circular_buffer), end(circular_buffer), [&](Request* m) {
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