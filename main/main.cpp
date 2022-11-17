#include <iostream>


//int example_send(  const std::string raw_ip_address = "127.0.0.1"  ,const unsigned short port_num = 3333)
//{
//    try {
//        SyncTCPClient client(raw_ip_address, port_num);
//        client.connect();
//        std::cout << "Sending request to the server... "<< std::endl;
//        std::string response = client.emulateLongComputationOp(10);
//        std::cout << "Response received: " << response << std::endl;
//        client.close();
//    }
//    catch (system::system_error &e) {
//        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
//        return e.code().value();
//    }
//    return 0;
//}
//
//nt example_rcv(  const std::string raw_ip_address = "127.0.0.1"  ,const unsigned short port_num = 3333)
//{
//    try {
//        SyncTCPClient client(raw_ip_address, port_num);
//        client.connect();
//        std::cout << "Sending request to the server... "<< std::endl;
//        std::string response = client.emulateLongComputationOp(10);
//        std::cout << "Response received: " << response << std::endl;
//        client.close();
//    }
//    catch (system::system_error &e) {
//        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
//        return e.code().value();
//    }
//    return 0;
//}

#include <lm_udp.h>

int main() {
    std::cout << "SANDBOX, Examples!" << std::endl;
    return 0;
}
