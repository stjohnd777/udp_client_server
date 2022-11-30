//
// Created by Daniel St. John on 11/21/22.
//


#include <net.h>

#include <string>
#include <iostream>
#include <tuple>
#include <memory>

using namespace std;
using namespace lm::spp;

//string string_handler(std::tuple<size_t, std::shared_ptr<char[]>> t) {
//
//    auto len = get<0>(t);
//    auto pChar = std::get<1>(t);
//
//    char s[MAX_DATAGRAM];
//    for (size_t idx = 0; idx < len; idx++) {
//        char* p = pChar.get();
//        s[idx] = *(pChar.get() + idx);
//    }
//    s[len] = 0;
//    string str(s,len);
//    cout << " Server Received " << str << endl;
//    return str;
//}

int main() {

    bool isRunning = true;
    std::string host = "127.0.0.1";
    unsigned short port = 7767;
    UdpUtilsSync udpUtil;
    while ( isRunning) {
        auto t = udpUtil.ServerReceiveNoReply(host, port);
        auto len = get<0>(t);
        auto pChar = std::get<1>(t);
        Request* req = lm::spp::DeSerialize<Request>(pChar.get());
        cout << "receive:" << req->seq << ":" << req->gpsTime << ":" << req->cameraId << endl;

    }

}
