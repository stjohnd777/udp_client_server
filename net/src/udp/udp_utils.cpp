
#include <boost/asio.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;

namespace lm {
    namespace  spp {

        UdpUtilsSync::UdpClientSync(const std::string &host, unsigned short port_num) : m_sock(m_ios) {

            m_ep = utils::GetEndpoint(host, port);
            m_sock.open(m_ep.protocol());
            m_sock.connect(m_ep);
        }

        void UdpUtilsSync::Send(const std::string &request) {

            write(m_sock, asio::buffer(request));
        }

        std::string UdpUtilsSync::Receive() {

            streambuf buf;
            read_until(m_sock, buf, '\n');
            istream input(&buf);
            string response;
            getline(input, response);
            return response;
        }


        UdpUtilsSync::~UdpClientSync() {
            m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            m_sock.close();
        }

    }
}




