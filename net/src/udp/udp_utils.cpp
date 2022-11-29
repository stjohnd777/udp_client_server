
#include <algorithm>
#include <boost/asio.hpp>

#include <utils.h>
#include <udp/udp_utils.h>



namespace lm {
    namespace  spp {

        using namespace std;
        using namespace boost;
        using namespace boost::asio;

        UdpUtilsSync::UdpUtilsSync(): 
            m_udp_socket(m_ios) {
            m_udp_socket.open(asio::ip::udp::v4());
        }

        void UdpUtilsSync::SendTo(std::string host, unsigned short port, const char* pdata, size_t len) {
            auto udp_ep = utils::GetUdpEndpoint(host, port);

            char  data[len];
            for (size_t index = 0; index < len; index++) {
                char c = *( pdata + index);
                data[index] = c;
            }

            auto bufferedData = asio::buffer(data,len);
            m_udp_socket.send_to(bufferedData,udp_ep);
        }

        std::tuple<size_t,std::shared_ptr<char[]> >
        UdpUtilsSync::BlockingRead() {

            char recv_buf[1024];
            memset(recv_buf,0,1024);

            boost::asio::ip::udp::endpoint sender_endpoint;
            size_t bytes_received = m_udp_socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);


            std::shared_ptr<char[]> sp(new char[bytes_received]);
            for (size_t index = 0; index < bytes_received; index++) {
                char c = recv_buf[index];
                *(sp.get()+index) = c;
            }

            return std::make_tuple(bytes_received,sp);
        }

        std::tuple<size_t,std::shared_ptr<char[]>>
        UdpUtilsSync::ServerReceiveNoReply(std::string host, unsigned short port) {

            boost::asio::io_context io_context;
            boost::asio::ip::udp::socket socket(io_context, boost::asio::ip::udp::endpoint( boost::asio::ip::udp::v4(),port));

            char recv_buf[1024];
            memset(recv_buf,0,1024);

            boost::asio::ip::udp::endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

            std::shared_ptr<char[]> sp(new char[bytes_received]);
            for (size_t index = 0; index < bytes_received; index++) {
                char c = recv_buf[index];
                *(sp.get()+index) = c;
            }
            return std::make_tuple(bytes_received,sp);
        }

        std::tuple<size_t,std::shared_ptr<char[]>> UdpUtilsSync::RequestReply(std::string host, unsigned short port, const char* pdata, size_t len) {
            SendTo(host,port,pdata,len);
            return BlockingRead();
        }

        void UdpUtilsSync::ReceiveReply(
                std::string host,
                unsigned short port,
                REQ_REPLY_HANDLER f) {
            using namespace boost::asio;
            using namespace boost::asio::ip;

            udp::socket socket(m_ios, boost::asio::ip::udp::endpoint( udp::v4(), port));
            boost::array<char, 1024> recv_buf;
            udp::endpoint remote_endpoint;

            std::size_t bytes_received = socket.receive_from(buffer(recv_buf), remote_endpoint);
            std::shared_ptr<char[]> sp(new char[bytes_received]);
            for (size_t index = 0; index < bytes_received; index++) {
                char c = recv_buf[index];
                *(sp.get()+index) = c;
            }
            auto req = std::make_tuple(bytes_received,sp);

            auto res = f(req);
            auto len = std::get<0>(res);
            auto pChar = std::get<1>(res);
            char s[len+1];
            for ( size_t idx =0; idx < len; idx++){
                char * p = pChar.get();
                s[idx] = *( pChar.get() + idx);
            }

            boost::system::error_code ignored_error;
            socket.send_to(buffer(s,len), remote_endpoint, 0, ignored_error);
        }

        UdpUtilsSync::~UdpUtilsSync() {
            m_udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            m_udp_socket.close();
        }

    }
}




