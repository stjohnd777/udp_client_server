#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <net.h>

template<class Req, class Reply >
class UdpServer {
public:
    std::shared_ptr<Req*> Receive(std::string host, unsigned short port, Req req) {
        auto data = udpUtil.ReceiveNoReply(host, port);
        Req* req = lm::spp::DeSerialize<Req>(data);
        return std::make_shared <Req*>(req);
    }
    std::shared_ptr<Reply*> ReceiveReply(std::string host, unsigned short port, std::function<Reply* (Req*)> handler) {
        char* data = udpUtil.RequestReply(host, port, data);
        Req* res = lm::spp::DeSerialize<Reply>(data);
        Reply* res = handler(res);
        return std::make_shared <Reply*>(res);
    }
    void StartReceiving(std::string host, unsigned short port, std::function<void(Req*)> handler) {
        isRunning = true;
        while (isRunning) {
            auto req = Receive(host, port, handler);
            handler(req);
        }
    }
    void StartReqRes(std::string host, unsigned short port, std::function<Reply* (Req*)> handler) {
        isRunning = true;
        while (isRunning) {
            ReceiveReply(host, port, handler)
        }
    }
    void Stop() {
        isRunning false;
    }
protected:
    lm::spp::UdpUtilsSync udpUtil;
    bool isRunning = false;
};

int main() {


    return 0;
}
