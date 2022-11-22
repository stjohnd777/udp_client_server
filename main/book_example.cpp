#include <boost/asio.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace boost;

class SyncUDPClient {

public:
    SyncUDPClient() : m_sock(m_ios) {
        m_sock.open(asio::ip::udp::v4());
    }

    std::string req_res(
            const std::string &ip,
            unsigned short port,
            const std::string &request) {

        asio::ip::udp::endpoint ep(asio::ip::address::from_string(ip),port);

        send(ep, request);
        return receive(ep);
    };

    void send(const asio::ip::udp::endpoint &ep, const std::string &request) {

        m_sock.send_to(asio::buffer(request), ep);
    }

    std::string receive(asio::ip::udp::endpoint &ep) {
        char response[6];
        m_sock.connect(ep);
        std::size_t bytes_received = m_sock.receive_from(asio::buffer(response), ep);
        m_sock.shutdown(asio::ip::udp::socket::shutdown_both);
        return std::string(response, bytes_received);
    }

private:
    asio::io_service m_ios;
    asio::ip::udp::socket m_sock;
};

int main() {
    const std::string ip = "127.0.0.1";
    const unsigned short port = 7767;
    asio::ip::udp::endpoint ep(asio::ip::address::from_string(ip),port);

    try {
        SyncUDPClient client;

        std::cout << "Sending request  ... " << std::endl;

        std::string request = "Request:Command:1:2:3 \n";

        client.send(ep, request);

        string response = client.receive(ep);

        std::cout << "Response from the server #1 received: " << response << std::endl;
    }
    catch (system::system_error &e) {
        std::cout << "Error! Error code = " << e.code() << ". Message: " << e.what();
        return e.code().value();
    }

    return 0;
}