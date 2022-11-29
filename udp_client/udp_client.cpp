#include <iostream>
#include <string>

#include <net.h>
#include <memory>
#include <algorithm>
#include <functional>

#include <boost/array.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

struct Request
{
    uint32_t seq;
    double gpsTime;
    double sunToRsoSrv[3];
    double chsToRsoSrv[4][4];
    double oofToEciPos[3];
    double oofToEciAtt[4];
    double lookatSrv[3];
} ;

struct Response
{
    uint32_t seq;
    double gpsTime;
    int code;
};


template<class Req, class Res >
class UdpClient {
public:
    UdpClient(std::string host, unsigned short port ): m_host(host),m_port(port) {
    }
    void Send( Req& req) {
        lm::spp::UdpSend<Request>(m_host, m_port, req);
    }

    std::shared_ptr<Res*> RequestReply( Req req) {
        return lm::spp::UdpRequestReply<Request, Response>(m_host, m_port, req);
    }

protected:
    std::string m_host;
    unsigned short m_port;
};





int main() {
    Request req;

    lm::spp::UdpSend<Request>("127.0.0.1",7767,req);
    auto res = lm::spp::UdpRequestReply<Request,Response>("127.0.0.1", 7767, req);

    UdpClient<Request, Response> udpClient("127.0.0.1",7767);
    udpClient.Send(req);

    auto res2 = udpClient.RequestReply(req);
    return 0;
}
//string img = "C:/Users/e438262/dev/github/udp_client_server/data/702HP_Satellite-Boeing.jpg";
//Mat image = imread(img,IMREAD_GRAYSCALE);
//imshow("Boing",image);
//cv::waitKey(0);