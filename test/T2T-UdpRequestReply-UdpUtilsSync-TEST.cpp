#define CATCH_CONFIG_MAIN // provides main(); this line is required in only one .cpp file
#include "catch.hpp"
#include <net.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <thread>
#include <tuple>
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



TEST_CASE( "T2T-RequestReply", "1" ) {
    REQUIRE( true == true );
    bool isRunning = true;
    std::string host = "127.0.0.1";
    unsigned short port = 7767;

        auto startClient = [isRunning](string host, unsigned short port) {
            auto t = new thread([&](string host, unsigned short port) {
                UdpUtilsSync udpUtil;
                int send_count = 0;
                int count = 0;
                while (isRunning) {

                    Request req;
                    req.seq = count;
                    req.gpsTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
                    req.cameraId = 1;
                    char* bytesReq = lm::spp::Serialize(req);
                    cout << "sending thread:" << std::this_thread::get_id()  << ":" << bytesReq << " len:" << sizeof(Request) << endl;
                    
                    auto tupleRes = udpUtil.RequestReply(host,port,bytesReq,sizeof(Request));

                    auto len_res = std::get<0>(tupleRes);
                    auto bytesRes = std::get<1>(tupleRes);
                    Response* res = lm::spp::DeSerialize<Response>(bytesRes.get());

                    std::this_thread::sleep_for(1000ms);
   
                }
                std::cout << "thread:" << std::this_thread::get_id()  << " Sender Exiting" << std::endl;
                },host,port );
            return t;
        };
        startClient(host,port)->detach();

        auto startServer = [isRunning](string host, unsigned short port) {
            auto t = new thread([&](string host, unsigned short port)  {
                UdpUtilsSync udpUtil;
                while (isRunning) {
                    
                    udpUtil.ReceiveReply(host, port, [&](std::tuple<size_t, std::shared_ptr<char[]>> req) {

                        // Get request Data
                        auto len = std::get<0>(req);
                        auto pChar = std::get<1>(req);
                        Request* request = lm::spp::DeSerialize<Request>(pChar.get());
                        cout << "receive:" << request->seq << ":" << request->gpsTime << ":" << request->cameraId << endl;

                        // Create Response
                        Response response;
                        response.seq = request->seq;
                        response.gpsTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
                        response.cameraId = request->cameraId;
                        response.retCode = 77;

                        // Serialize Response
                        auto bytesOut = Serialize<Response>(response);
                        size_t size_bytes_out = sizeof(Response);
                        std::shared_ptr<char[]> sp(new char[size_bytes_out]);
                        memcpy(sp.get(), bytesOut, size_bytes_out);

                        auto res = std::make_tuple(size_bytes_out, sp);
                        return res;
                        });

                }
                std::cout << "Server Thread exiting" << std::endl;
             },host,port);
            return t;
        };
        startServer(host,port)->detach();

        HaltMainForSec(60);
        isRunning = false;
        HaltMainForSec(5);
}




//template<class Req, class Res>
//Res* provider(Req* req) {
//    static uint32_t counter = 0;
//
//     Do you processing on Request
//     ... stuff here
//     ... construct you response
//
//    Res* res = new Res();
//    res->seq = counter;
//    res->cameraId = req->cameraId;
//    res->gpsTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
//    res->retCode = counter;
//    counter++;
//    return res;
//};

auto provider = [&](Request* req) -> Response* {
    static uint32_t counter = 0;

    // Do you processing on Request
    // ... stuff here
    // ... construct you response

    Response* res = new Response();
    res->seq = counter;
    res->cameraId = req->cameraId;
    res->gpsTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    res->retCode = counter;
    counter++;
    return res;
};

TEST_CASE("T2T-RequestReply", "2") {
    REQUIRE(true == true);

    std::string host = "127.0.0.1";
    unsigned short port = 7767;

    // Server listening 
    //UdpServer<Request, Response> server(host, port);
    //bool isRunningAsThread = true;
    //server.StartReqRes(provider , isRunningAsThread);

    UdpClient<Request, Response> client(host, port);
    for (auto i = 0; i < 100; i++) {
        Request req;
        req.seq = i;
        req.gpsTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        req.cameraId = 1;
        auto spRes   = client.RequestReply(req);
        //Response* res = spRes.get();
    }

    //server.Stop();
}