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
        std::string host = "127.0.0.1";
        unsigned short port = 7767;
        int COUNT = 100;
        UdpUtilsSync *udpUtil = new UdpUtilsSync();
        for (int i = 0; i < COUNT; i++) {
            stringstream ss;
            ss << "Sending Some Data " << i << endl;
            auto t = udpUtil->RequestReply(host, port, ss.str().c_str(), ss.str().length() );
            auto len = std::get<0>(t);
            auto pChar = std::get<1>(t);
            char s[len+1];
            for ( size_t idx =0; idx < len; idx++){
                char * p = pChar.get();
                s[idx] = *( pChar.get() + idx);
            }
            s[len] = 0;
            string str(s);
            cout << "reply:" << s << endl;
        }
        cout << "cleaning up" << endl;
        delete udpUtil;
    }catch (std::exception& ex){
        cerr << ex.what() << endl;
    }

}
