#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

#include <net.h>

using namespace std;
int main() {

    UdpUtilsSync udpUtil("localhost", 7767);

    bool isRunning = true;

    std::thread t([&]() {
        while (isRunning){
            udpUtil.Send("Some Data");
            std::this_thread::sleep_for(1000ms);
        }
    });

    std::thread t([&]() {
        while (isRunning){
            auto data = udpUtil.Recieve();
        }
    });

    std::this_thread::sleep_for(30* 1000ms);

    isRunning = false;

}
