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


 
int main() {

   
    string img = "C:/Users/e438262/dev/github/udp_client_server/data/702HP_Satellite-Boeing.jpg";
    Mat image = imread(img,IMREAD_GRAYSCALE);
    imshow("Boing",image);
    cv::waitKey(0);

    Request req;
    lm::spp::UdpSend<Request>("127.0.0.1",7767,req);
    auto res = lm::spp::UdpRequestReply<Request,Response>("127.0.0.1", 7767, req);

    lm::spp::UdpClient<Request, Response> udpClient("127.0.0.1",7767);
    udpClient.Send(req);

    auto res2 = udpClient.RequestReply(req);
    return 0;
}
