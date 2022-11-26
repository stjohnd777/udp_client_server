#include <iostream>
#include <string>

#include <net.h>
#include <memory>
#include <functional>

template<class Req, class Res >
class UdpClient {

    static 
    void Send(std::string host, unsigned short port, Req req  ) {
        UdpUtilsSync udpUtil;
        char* data = tup = lm::spp::Seriae(req);
        udpUtil.RequestAndForgets(host, port, data );
    }

    static 
    std::shared_ptr<Res*> RequestReply(std::string host, unsigned short port) {
        char* data = udpUtil.RequestReply(host, port, data);
        Reply* res = lm::spp::DeSerialize<Reply>(data);
        return std::make_shared <Reply*>(res);
    }
};

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main() {
    Mat image = imread("/Users/overman/dev/github/_mycode/_cpp/upd/data/702HP_Satellite-Boeing.jpg",IMREAD_GRAYSCALE);
    return 0;
}
