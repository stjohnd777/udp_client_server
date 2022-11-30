//
// Created by Daniel St. John on 11/21/22.
//


#include <net.h>

#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <chrono>
 

using namespace std;
using namespace lm::spp;
using namespace std::chrono;

int main() {

    try {
        std::string host = "127.0.0.1";
        unsigned short port = 7767;
        int COUNT = 1000;
        UdpUtilsSync *udpUtil = new UdpUtilsSync();
        for (int i = 0; i < COUNT; i++) {
            Request req;
            req.seq = i;
            req.gpsTime =  duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            req.cameraId = 1;
            auto bytes = lm::spp::Serialize(req);
            stringstream ss;
            ss << "Sending Some Data " << bytes << endl;
            udpUtil->SendTo(host, port,bytes,sizeof(Request));
            cout << "sent:" << req.seq << ":" << req.gpsTime << ":" << req.cameraId << endl;
            std::this_thread::sleep_for(1000ms);
        }
        cout << "cleaning up" << endl;
        delete udpUtil;
    }catch (std::exception& ex){
        cerr << ex.what() << endl;
    }

}
