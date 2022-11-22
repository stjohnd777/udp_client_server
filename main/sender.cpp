//
// Created by Daniel St. John on 11/21/22.
//


#include <net.h>

#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <boost/circular_buffer.hpp>

using namespace std;
using namespace lm::spp;

int main() {

    try {
        bool isRunning = true;
        std::string host = "127.0.0.1";
        unsigned short port = 7767;
        UdpUtilsSync *udpUtil = new UdpUtilsSync();
        for (int i = 0; i < 10; i++) {
            stringstream ss;
            ss << "Sending Some Data " << i << endl;
            udpUtil->ClientSendAndForget(host, port, ss.str());
            cout << "Sent " << ss.str() << endl;
            std::this_thread::sleep_for(1000ms);
        }
        cout << "cleaning up" << endl;
        delete udpUtil;
    }catch (std::exception& ex){
        cerr << ex.what() << endl;
    }

}
