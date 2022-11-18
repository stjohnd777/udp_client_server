#include <udp_client.h>
#include <boost/asio.hpp>

using namespace boost;

UdpClientSync::UdpClientSync(const std::string &raw_ip_address, unsigned short port_num) :
        m_ep(asio::ip::address::from_string(raw_ip_address), port_num),
        m_sock(m_ios) {
    m_sock.open(m_ep.protocol());
    m_sock.connect(m_ep);
}

void UdpClientSync::fireAndForget(const std::string &request) {
    asio::write(m_sock, asio::buffer(request));
}

UdpClientSync::~UdpClientSync() {
    m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    m_sock.close();
}




