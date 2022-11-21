
#include <boost/asio.hpp>

#include <utils.h>
#include <udp/udp_utils.h>



namespace lm {
    namespace  spp {

        using namespace std;
        using namespace boost;
        using namespace boost::asio;

        UdpUtilsSync::UdpUtilsSync() : m_sock(m_ios) {

            // Because communication over UDP protocol, 
            // which is a connectionless protocol, a single 
            // object of this class can be used to communicate 
            // with multiple servers.
            m_sock.open(asio::ip::udp::v4());
        }

        void UdpUtilsSync::Send(string host, unsigned short port,const std::string &request) {
            auto udp_ep = utils::GetUdpEndpoint(host, port);
            auto bufferedData = asio::buffer(request);
            m_sock.send_to(bufferedData,udp_ep);
           
        }

        std::string UdpUtilsSync::Recieve(string host, unsigned short port) {
            // This method copies a datagram that came from the server designated by the sender_endpoint 
            // object to the buffer specified by the buffers argument.
            auto udp_ep = utils::GetUdpEndpoint(host, port);
            char response[1024];
            // if the size of the datagram that arrives larger than the size of the supplied buffer, the method will fail.
            std::size_t bytes_recieved = m_sock.receive_from(asio::buffer(response), udp_ep);
            //  If the datagram never arrives the method will never unblock (hangs the  application)
            m_sock.shutdown(asio::ip::udp::socket::shutdown_both);
            return std::string(response, bytes_recieved);
        }


        UdpUtilsSync::~UdpUtilsSync() {
            m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            m_sock.close();
        }

    }
}




