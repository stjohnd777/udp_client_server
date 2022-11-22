
#include <boost/asio.hpp>

#include <utils.h>
#include <udp/udp_utils.h>



namespace lm {
    namespace  spp {

        using namespace std;
        using namespace boost;
        using namespace boost::asio;

        UdpUtilsSync::UdpUtilsSync() : m_udp_socket(m_ios) {

            // Because communication over UDP protocol, 
            // which is a connectionless protocol, a single 
            // object of this class can be used to communicate 
            // with multiple servers.
            m_udp_socket.open(asio::ip::udp::v4());
        }

        void UdpUtilsSync::RequestAndForget(string host, unsigned short port, const std::string &data) {
            auto udp_ep = utils::GetUdpEndpoint(host, port);
            auto bufferedData = asio::buffer(data);
            m_udp_socket.send_to(bufferedData,udp_ep);
        }

        void UdpUtilsSync::SendTo(std::string host, unsigned short port, const std::string& data) {
            auto udp_ep = utils::GetUdpEndpoint(host, port);
            auto bufferedData = asio::buffer(data);
            m_udp_socket.send_to(bufferedData, udp_ep);
        }

        string UdpUtilsSync::ListenAndReply() {
            char response[1024];
            boost::asio::ip::udp::endpoint sender_endpoint;
            size_t bytes_received = m_udp_socket.receive_from(boost::asio::buffer(response), sender_endpoint);
            std::string rcvData(response, bytes_received);
            return rcvData;
        }

        string UdpUtilsSync::RequestReply(std::string host, unsigned short port, const std::string& data) {
            SendTo(host,port,data);
            return ListenAndReply();
        }

        std::string UdpUtilsSync::ClientReceive(string host, unsigned short port) {

            asio::ip::udp::endpoint from_ep; // senders ep
            char response[1024];
            memset(response, 0, sizeof(response));
            strcpy(response, "This is Some Bugus Reply");
            // if the size of the datagram that arrives larger than the size of the supplied buffer, the method will fail.
            std::size_t bytes_received = m_udp_socket.receive_from(asio::buffer(response), from_ep);
            //  If the datagram never arrives the method will never unblock (hangs the  application)
            //m_sock.shutdown(asio::ip::udp::socket::shutdown_both);
            return std::string(response, bytes_received);
        }

        std::string UdpUtilsSync::ReceiveNoReply(std::string host, unsigned short port) {
 
            boost::asio::io_context io_context;
            boost::asio::ip::udp::socket socket(
                io_context, 
                boost::asio::ip::udp::endpoint(
                boost::asio::ip::udp::v4(),
                port));

            boost::array<char, 24> recv_buf;
            boost::asio::ip::udp::endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
            std::string data(recv_buf.begin(), bytes_received);

            return data;
        }

        std::string UdpUtilsSync::ReceiveAndReply(std::string host, unsigned short port) {
            boost::asio::ip::udp::socket socket(m_ios, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));
            boost::array<char, 24> recv_buf;
            boost::asio::ip::udp::endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
            std::string data(recv_buf.begin(), bytes_received);

            std::string reply = "200:OK";
            boost::system::error_code ignored_error;
            socket.send_to(boost::asio::buffer(reply), remote_endpoint, 0, ignored_error);

            return data;
        }


        UdpUtilsSync::~UdpUtilsSync() {
            m_udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            m_udp_socket.close();
        }

    }
}




