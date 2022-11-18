#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

#include <lm_udp.h>

using namespace std;
int main() {

    UdpServerSync server("localhost", 7767);

    auto f = [&]() {
        server.start([&](string data) {
            static unsigned long counter = 1;
            cout << data << ":" << counter << endl;
        });
    };

    std::thread t(f);
    //t.detach();

    UdpClientSync udpClient ( "localhost", 7767);
    for ( int i = 0; i < 1000 ; i++){
        stringstream ss;
        ss << "client sends message " << i << endl;
        udpClient.fireAndForget(ss.str());
        std::this_thread::sleep_for(1000ms);
    }
}
