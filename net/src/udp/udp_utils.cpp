
#include <boost/asio.hpp>

#include <utils.h>
#include <udp/udp_utils.h>



namespace lm {
    namespace  spp {

        using namespace std;
        using namespace boost;
        using namespace boost::asio;


        template<std::size_t N,std::size_t M >
        UdpUtilsSync<N,M>::UdpUtilsSync() : m_udp_socket(m_ios) {
            // Because communication over UDP protocol, 
            // which is a connectionless protocol, a single 
            // object of this class can be used to communicate 
            // with multiple servers.
            m_udp_socket.open(asio::ip::udp::v4());
        }


        template<std::size_t N , std::size_t M>
        void UdpUtilsSync<N,M>::SendTo(std::string host, unsigned short port, const boost::array<char, N>& data){
            auto udp_ep = utils::GetUdpEndpoint(host, port);
            auto bufferedData = asio::buffer(data);
            m_udp_socket.send_to(bufferedData, udp_ep);
        }


        template<std::size_t N ,std::size_t M>
        boost::array<char, N>  UdpUtilsSync<N,M>::BlockingRead() {
            char response[N];
            boost::asio::ip::udp::endpoint sender_endpoint;
            size_t bytes_received = m_udp_socket.receive_from(boost::asio::buffer(response), sender_endpoint);
            boost::array<char, N> data;
            for ( uint8_t index =0 ;index < N;index++){
                if ( index < bytes_received){
                    data[index] = response[index];
                }else {
                    data[index] = 0;
                }
            }
            return data;
        }


        template<std::size_t N, std::size_t M >
        boost::array<char,M> UdpUtilsSync<N,M>::RequestReply(std::string host, unsigned short port, const boost::array<char, N>& data) {
            SendTo(host,port,data);
            return BlockingRead();
        }

        template<std::size_t N, std::size_t M >
        boost::array<char, N> UdpUtilsSync<N,M>::ClientReceive(string host, unsigned short port) {

            asio::ip::udp::endpoint from_ep; // senders ep
            char response[M];
            memset(response, 0, sizeof(response));
            strcpy(response, "This is Some Bugus Reply");
            // if the size of the datagram that arrives larger than the size of the supplied buffer, the method will fail.
            std::size_t bytes_received = m_udp_socket.receive_from(asio::buffer(response), from_ep);
            boost::array<char, N> data;
            for ( uint8_t index =0 ; index < N; index++){
                if ( index < bytes_received){
                    data[index] = response[index];
                }else {
                    data[index] = 0;
                }
            }
            return data;
        }




        template<std::size_t N, std::size_t M >
        boost::array<char, N> UdpUtilsSync<N,M>::ReceiveNoReply(std::string host, unsigned short port) {

            boost::asio::io_context io_context;
            boost::asio::ip::udp::socket socket(
                    io_context,
                    boost::asio::ip::udp::endpoint(
                            boost::asio::ip::udp::v4(),
                            port));

            boost::array<char, N> recv_buf;
            boost::asio::ip::udp::endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

            return recv_buf;
        }



        template<std::size_t N , std::size_t M>
        void UdpUtilsSync<N,M>::ReceiveAndReply(
                std::string host,
                unsigned short port,
                std::function<boost::array<char, M>(boost::array<char,N>)> f) {
            boost::asio::ip::udp::socket socket(m_ios, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));
            boost::array<char, N> recv_buf;
            boost::asio::ip::udp::endpoint remote_endpoint;
            std::size_t bytes_received = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

            boost::array<char, M> reply;
            reply[0] = 'O';
            reply[1] = 'K';
            boost::system::error_code ignored_error;
            socket.send_to(boost::asio::buffer(reply), remote_endpoint, 0, ignored_error);


        }

        template<std::size_t N, std::size_t M >
        UdpUtilsSync<N,M>::~UdpUtilsSync() {
            m_udp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            m_udp_socket.close();
        }

    }
}




