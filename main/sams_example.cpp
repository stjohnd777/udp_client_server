#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace boost::asio::ip;
//using boost::asio::ip::udp;
using namespace std;

namespace sam {

    /*
    template<typename MESSAGE, typename RT, typename CALLABLE, typename... Args>
    class MessageHandler<MESSAGE, RT(CALLABLE::*)(Args...) const> : public MessageHandlerBase<MESSAGE>
    {
    public:
        MessageHandler(CALLABLE callable_) : callable(callable_) { }
        virtual int arguments() { return sizeof...(Args); }
        virtual int invoke(MESSAGE& message) { return invoke(message, index_sequence_for<Args...>{}); }

        template<size_t...Index>
        inline int invoke(MESSAGE& message, index_sequence<Index...>)
        {
            std::tuple<MessageArgument<MESSAGE, nth_type_of<Index, Args...>>...> args{ {message, 1 + Index}... };
            return callable(std::get<Index>(args)...)
        }
    protected:
        CALLABLE callable;
    };
    */


class GNCSpeedGoatUDPServer {

public:

    GNCSpeedGoatUDPServer(boost::asio::io_service& io_service, address v4address, int port) 
    : _socket(io_service, udp::endpoint(v4address, port)) {	
        startReceive();
    }

private:

    void startReceive() {

        _socket.async_receive_from(

            boost::asio::buffer(_recvBuffer), 
            _remoteEndpoint,
            [this](const boost::system::error_code& error, std::size_t bytes_transferred){
                this->handleReceive(error,bytes_transferred);
            }
        );
    }

    void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred) {

        std::array<char, 1024> _recvBuffer;

		std::cout << "-> | " ;

        if (!error || error == boost::asio::error::message_size) {

            std::string rmsg(_recvBuffer.begin(), recv_buffer.begin() + bytes_transferred);
        
            std::cout << rmsg;

            //auto response = std::make_shared<std::string>("ack\n");
						
            //_socket.async_send_to(
            //    
            //    boost::asio::buffer(*response), 
            //    _remoteEndpoint,
            //    [this](std::shared_ptr<std::string> message, const boost::system::error_code& ec,std::size_t bytes_transferred){
            //        this->handleSend(message,ec,bytes_transferred);
            //    }
            //);
        }
    }

    void handleSend(std::shared_ptr<std::string> message, const boost::system::error_code& ec,std::size_t bytes_transferred) {
				
		std::cout << "<- " << message << std::endl;
        startReceive();
    }

    udp::socket _socket;
    udp::endpoint _remoteEndpoint;
    std::array<char, 1024> _recvBuffer;
};

}  // namespace

int main() {

    try {

        boost::asio::io_service io_service;

        // auto addr = udp::v4()  // 0.0.0.0
        auto addr = address::from_string("160.205.159.60"); // commit to config
        cout << addr << endl;
        int port = 1111;

     
        GNCSpeedGoatUDPServer server (io_service, addr, port);
				
		std::cout << "SpeedGoat UDP Server is listeing at:  " << addr << ":" << port << std::endl;
        
        io_service.run();

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}
 