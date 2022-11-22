
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

        void UdpUtilsSync::ClientSendAndForget(string host, unsigned short port, const std::string &data) {
            auto udp_ep = utils::GetUdpEndpoint(host, port);
            auto bufferedData = asio::buffer(data);
            m_sock.send_to(bufferedData,udp_ep);
        }

        std::string UdpUtilsSync::ClientReceive(string host, unsigned short port) {

            asio::ip::udp::endpoint from_ep; // senders ep
            char response[1024];
            // if the size of the datagram that arrives larger than the size of the supplied buffer, the method will fail.
            std::size_t bytes_received = m_sock.receive_from(asio::buffer(response), from_ep);
            //  If the datagram never arrives the method will never unblock (hangs the  application)
            //m_sock.shutdown(asio::ip::udp::socket::shutdown_both);
            return std::string(response, bytes_received);
        }


        UdpUtilsSync::~UdpUtilsSync() {
            m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            m_sock.close();
        }

    }
}




