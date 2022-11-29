
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
            boost::array<char, 1024> data;
            int index = 0;
            for (size_t index = 0; index < len; len++) {
                data[index] = *( pdata + index);
            }
            auto bufferedData = asio::buffer(data);
            m_udp_socket.send_to(bufferedData,udp_ep);
        }

        boost::array<char, 1024>  UdpUtilsSync::BlockingRead() {
            char response[1024];
            boost::asio::ip::udp::endpoint sender_endpoint;
            size_t bytes_received = m_udp_socket.receive_from(boost::asio::buffer(response), sender_endpoint);
            boost::array<char, 1024> data;
            for ( uint8_t index =0 ;index < 1024;index++){
                if ( index < bytes_received){
                    data[index] = response[index];
                }else {
                    data[index] = 0;
                }
            }
            return data;
        }

 
        boost::array<char, 1024>  ReceiveNoReply(std::string host, unsigned short port) {

            boost::asio::io_context io_context;
            boost::asio::ip::udp::socket socket(io_context, boost::asio::ip::udp::endpoint( boost::asio::ip::udp::v4(),port));

            boost::array<char, 1024> recv_buf;
            boost::asio::ip::udp::endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

            return recv_buf;
        }





        boost::array<char,1024> UdpUtilsSync::RequestReply(std::string host, unsigned short port, char* pdata, size_t len) {
            SendTo(host,port,pdata,len);
            return BlockingRead();
        }

        void UdpUtilsSync::ReceiveReply( std::string host,unsigned short port,function<boost::array<char, 1024>(boost::array<char,1024>)> f) {
            using namespace boost::asio;
            using namespace boost::asio::ip;

            udp::socket socket(m_ios, boost::asio::ip::udp::endpoint( udp::v4(), port));
            boost::array<char, 1024> recv_buf;
            udp::endpoint remote_endpoint;

            std::size_t bytes_received = socket.receive_from(buffer(recv_buf), remote_endpoint);

            boost::array<char, 1024> res = f(recv_buf);

            boost::system::error_code ignored_error;
            socket.send_to(buffer(res), remote_endpoint, 0, ignored_error);
        }

 
        UdpUtilsSync::~UdpUtilsSync() {
            m_udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            m_udp_socket.close();
        }

    }
}




