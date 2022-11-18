//
// Created by Daniel St. John on 11/17/22.
//
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace boost;

class SyncTCPClient {
public:
    SyncTCPClient(const std::string& _ip,unsigned short port) {

        using asio::ip;
        using asio::ip::tcp;
        using  boost::system;
        address ip = address::from_string(_ip);
        m_ep = endpoint (ip,port);
        m_sock  = new socket(m_ios);
        m_sock->open(m_ep.protocol());
        error_code err;
        m_sock->connect(m_ep);


    }

    ~SyncTCPClient(){
        m_sock->shutdown( boost::asio::ip::tcp::socket::shutdown_both);
        m_sock->close();
        delete m_sock;
    }

    void sendRequest(const std::string& request) {
        asio::write(*m_sock, asio::buffer(request));
    }

    std::string receiveResponse() {
        asio::streambuf buf;
        asio::read_until(*m_sock, buf, '\n');
        std::istream input(&buf);
        std::string response;
        std::getline(input, response);
        return response;
    }

private:
    asio::io_service m_ios;
    asio::ip::tcp::endpoint m_ep;
    asio::ip::tcp::socket* m_sock;
};

int main()
{
    const string raw_ip_address = "127.0.0.1";
    const unsigned short port_num = 8989;

    SyncTCPClient client(raw_ip_address, port_num);

    std::string sendData = "Data \n";
    client.sendRequest(sendData);
    string recvData = client.receiveResponse();
    std::cout << "Response received: " << recvData << std::endl;


    return 0;
}