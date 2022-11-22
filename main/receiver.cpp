//
// Created by Daniel St. John on 11/21/22.
//


#include <net.h>

#include <string>
#include <iostream>

using namespace std;
using namespace lm::spp;

int main() {

    bool isRunning = true;
    std::string host = "127.0.0.1";
    unsigned short port = 7767;
    UdpUtilsSync udpUtil;
    while ( isRunning) {
        string data = udpUtil.ClientReceive(host, port);
        cout << "Received : " << data << endl;
    }


}
