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
        auto tuple = udpUtil.ServerReceiveNoReply(host, port);
        auto len = std::get<0>(tuple);
        auto pChar = std::get<1>(tuple);
        char s[len+1];
        for ( size_t idx =0; idx < len; idx++){
            char * p = pChar.get();
            s[idx] = *( pChar.get() + idx);
        }
        s[len] = 0;
        string str(s);
        cout << " Server Received " << str << endl;

    }

}
