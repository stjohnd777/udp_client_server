#include <udp_server.h>

#include <boost/asio.hpp>

using namespace boost;

UdpServerSync::UdpServerSync(const std::string &raw_ip_address, unsigned short port_num)
: m_sock(m_ios)
{
    m_ep = asio::ip::tcp::endpoint (asio::ip::address::from_string(raw_ip_address), port_num);
    m_sock.open(m_ep.protocol());
    m_sock.connect(m_ep);
}

void UdpServerSync::start(std::function<void(std::string)> f) {
    while (isRunning) {
        std::string  msg = receive();
        f(msg);
    }
}

std::string UdpServerSync::receive() {
    asio::streambuf buf;
    asio::read_until(m_sock, buf, '\n');
    std::istream input(&buf);
    std::string response;
    std::getline(input, response);
    return response;
}

UdpServerSync::~UdpServerSync() {
    m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    m_sock.close();
}


