#define CATCH_CONFIG_MAIN // provides main(); this line is required in only one .cpp file
#include "catch.hpp"

#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>

#include <net.h>

using namespace std;
using namespace lm::spp;
using namespace std::chrono;


std::string host = "127.0.0.1";
unsigned short port = 7767;

TEST_CASE( "UdpSendTempate-Test", "1" ) {
    REQUIRE( true == true );

    std::vector<Request> vsent;
    std::vector<Request*> vrecv;
 
    bool running = true;
    std::thread t([&]() {
        while (running) {
            UpdConsumeOne<Request>(host, port, [&](Request* r) {
                // consider shared_ptr
                vrecv.push_back(r);
                cout << "Conumer Request[" << r->seq << "," << r->gpsTime << "," << r->cameraId << "]" << endl;
                if (r->seq == 9) {
                    running = false;
                }
             });
        }
     });

    for ( auto i =0; i < 10 ; i++){
        Request req; // local temp
        req.seq =i;
        req.gpsTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        req.cameraId = i;
        UdpSend<Request>(host,port,req);
        vsent.push_back(req); // creates copy of object
        cout << "Send Request[" << req.seq << "," << req.gpsTime << "," << req.cameraId << "]" << endl;
        std::this_thread::sleep_for(1000ms);
    }

    running = false;
    t.join();

    //size_t send_count = vsend.size();
    //size_t rcv_count = vrecv.size();
    REQUIRE( vsent.size() == vrecv.size());

    for_each(begin(vsent), end(vsent), [&](Request req) {
       
        auto seq = req.seq;
        auto it = find(vrecv.begin(), vrecv.end(), [&](Request* _req) {
            return _req->seq == seq;
        });
        REQUIRE(it != end(vrecv));
        Request* _req= *it;
        REQUIRE( _req->cameraId == req.cameraId);
        REQUIRE(_req->gpsTime == req.gpsTime);


    });

    cout << "Exit Test" << endl;

}