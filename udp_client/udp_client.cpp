#include <iostream>
#include <string>

#include <net.h>
#include <memory>
#include <functional>

#include <boost/array.hpp>

template<class Req, class Res >
class UdpClient {

    static 
    void Send(std::string host, unsigned short port, Req req  ) {
        const size_t N = sizeof(Req);
        lm::spp::UdpUtilsSync<N> udpUtil;
        char* _data  = lm::spp::Serialize(req);
        boost::array<char, N> data;
        udpUtil.RequestAndForget(host, port, data );
    }

    static 
    std::shared_ptr<Res*> RequestReply(std::string host, unsigned short port, Req req) {
        const size_t N = sizeof(Req);
        lm::spp::UdpUtilsSync<N> udpUtil;
        char* _data  = lm::spp::Serialize(req);
        boost::array<char, N> data;
        for ( size_t index = 0 ; index < N ; index++){
            data[index] = _data[index];
        }
        boost::array<char, 1024> reply = udpUtil.RequestReply(host, port, data);

        Res* res = lm::spp::DeSerialize<Res>(reply);
        return std::make_shared <Res*>(res);
    }
};

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main() {
    Mat image = imread("/Users/overman/dev/github/_mycode/_cpp/upd/data/702HP_Satellite-Boeing.jpg",IMREAD_GRAYSCALE);
    imshow("Boing",image);
    return 0;
}
