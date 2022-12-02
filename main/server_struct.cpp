//
// Created by Daniel St. John on 11/21/22.
//


#include <net.h>

#include <string>
#include <iostream>
#include <chrono>
#include <tuple>
#include <memory>
#include <boost/array.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace lm::spp;
using namespace std::chrono;


int main() {

    bool isRunning = true;
    std::string host = "127.0.0.1";
    unsigned short port = 7767;


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
        cout << "server running" << endl;
        while (running) {
            cout << "server blocking read" << endl;
           UpdRecieveReplyOne<Request,Response>(host,port,provider);
           cout << "process request" << endl;
        }
        cout << "server exiting" << endl;
     });

    t.join();

    return 0;

}
