#define CATCH_CONFIG_MAIN // provides main(); this line is required in only one .cpp file
#include "catch.hpp"

#include <iostream>
#include <sstream>
#include <chrono>

#include <net.h>

using namespace std;
using namespace lm::spp;
using namespace std::chrono;

std::string host = "127.0.0.1";
unsigned short port = 7767;

TEST_CASE( "UdpRequestReplyTemplateFunction-Test", "1" ) {
    
    REQUIRE( true == true );


    auto provider = [&](Request * req){
        Response* res = new Response();
        res->seq = req->seq;
        res->gpsTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        res->cameraId = req->cameraId;
        res->retCode = 99;
        return res;
    };
 
    bool running = true;
    std::thread t([&]() {
        UpdRecieveReplyOne<Request, Response>(host, port, provider);
        while (running) {
            UpdRecieveReplyOne<Request,Response>(host,port,provider);
        }
     });

     for ( auto i =0; i < 10 ; i++){

        Request req;
        req.seq =i;
        req.gpsTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        req.cameraId = 1;
        cout << "Send Request[" << req.seq << "," << req.gpsTime << "," << req.cameraId << "]" << endl;
        
        auto res = UdpRequestReply<Request,Response>(host, port,req);
        cout << "Get Response Request[" << res->seq << "," << res->gpsTime << "," << res->cameraId << ":" << res->retCode << "]" << endl;
        
        std::this_thread::sleep_for(10000ms);
    }

    running = false;
    t.join();

    cout << "Exit Test" << endl;

}