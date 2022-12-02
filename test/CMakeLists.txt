project(test)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 CONFIG REQUIRED)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


################################################################
# Nameing Conventions
# T2T (Thread 2 Thread)
# P2P (Process 2 Process)
# All Catch2 Test end with executable name _TEST
# All intependent execuatables end woith _MAIN
# cmake target uses _ and cpp code uses - to seperate naming and the namin match up to underscore


################################################################
# Template to Copy
# Test is catch2 is working
################################################################
add_executable(Hello_TEST catch.hpp Hello-TEST.cpp)
target_include_directories(Hello_TEST PUBLIC net)
target_link_libraries(Hello_TEST  PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen)
################################################################


################################################################
# Serialize/Deserialize structs
#
################################################################
add_executable(Serializer_TEST catch.hpp Serializer-TEST.cpp)
target_include_directories(Serializer_TEST PUBLIC net)
target_link_libraries(Serializer_TEST  PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen)
################################################################



# Procuder Consumer Section

################################################################
# Test UpdUtilsSync Producer-Consumer Thread to Thread
#   Producer
#   void SendTo(std::string host, unsigned short port,const char* pdata, size_t len)
#   Consumer
#   std::tuple<size_t,std::shared_ptr<char[]> >ServerReceiveNoReply(std::string host, unsigned short port)
###############################################################
add_executable(T2T_UdpProducerConsumer_UdpUtilsSync_TEST catch.hpp T2T-UdpProducerConsumer-UdpUtilsSync-TEST.cpp)
target_include_directories(T2T_UdpProducerConsumer_UdpUtilsSync_TEST PUBLIC net)
target_link_libraries(T2T_UdpProducerConsumer_UdpUtilsSync_TEST PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen  )


################################################################
# Test Template Funtion Thread 2 Thread
#   UpdConsumeOne<>
#   UdpSend<>
################################################################
add_executable(T2T_UdpProducerConsumer_TempateFunctions_TEST catch.hpp T2T-UdpProducerConsumer-TemplateFunction-TEST.cpp)
target_include_directories( T2T_UdpProducerConsumer_TempateFunctions_TEST PUBLIC net)
target_link_libraries ( T2T_UdpProducerConsumer_TempateFunctions_TEST PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen )


# Request Reply Section

################################################################
# Test UpdUtilsSync Request-Response Thread to Thread
# client thread
# std::tuple<size_t,std::shared_ptr<char[]>>
# RequestReply(std::string host, unsigned short port, const char* pdata, size_t len);
# server/service thread
# void ReceiveReply(std::string host, unsigned short port ,  REQ_REPLY_HANDLER handler);
################################################################
add_executable(T2T-RequestReply-UdpUtilsSync-TEST catch.hpp T2T-UdpRequestReply-UdpUtilsSync-TEST.cpp)
target_include_directories(T2T-RequestReply-UdpUtilsSync-TEST PUBLIC net)
target_link_libraries(T2T-RequestReply-UdpUtilsSync-TEST PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen  )
 

################################################################
# Test UDP Request-Response Thread 2 Thread
# UdpServer<*,*>
# UdpClient<*,*>
#
################################################################
add_executable(T2T_UdpRequestReply_TemplateFunction_TEST catch.hpp T2T-UdpRequestReply-TemplateFunction-TEST.cpp)
target_include_directories(T2T_UdpRequestReply_TemplateFunction_TEST PUBLIC net)
target_link_libraries(T2T_UdpRequestReply_TemplateFunction_TEST PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen  )


 