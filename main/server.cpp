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

        udpUtil.ReceiveReply(host, port, [&]( std::tuple<size_t,std::shared_ptr<char[]>> req) {

            // Get request Data
            auto len = std::get<0>(req);
            auto pChar = std::get<1>(req);

            char s[len+1];
            for ( size_t idx =0; idx < len; idx++){
                char * p = pChar.get();
                s[idx] = *( pChar.get() + idx);
            }
            s[len] = 0;
            string str(s);
            cout << "server received: " << str << endl;

            // Process

            // Construct Reply
            size_t bytes_out = 2;
            std::shared_ptr<char[]> sp(new char[bytes_out]);
            *(sp.get()+0) = 'O';
            *(sp.get()+1) = 'K';

            auto res = std::make_tuple(bytes_out,sp);
            return res;
        });
    }

    return 0;

}
