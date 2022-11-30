project(mains)
set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 CONFIG REQUIRED)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

################################################################
# Thread to Thread Produced Comsumer
################################################################
add_executable(udp_send_rcv_example my_udp_send_rcv_example.cpp)
target_include_directories(
        udp_send_rcv_example
        PUBLIC
        net
)
target_link_libraries(udp_send_rcv_example  PRIVATE net )
 

################################################################
# Process to Process Procucer Consumer
################################################################
add_executable(sender sender.cpp)
target_include_directories(
        sender
        PUBLIC
        net
)
target_link_libraries(sender  PRIVATE net )


add_executable(receiver receiver.cpp)
target_include_directories(
        receiver
        PUBLIC
        net
)
target_link_libraries(receiver  PRIVATE net )
################################################################


################################################################
# Procees:Client Request/Reply Process:Server
################################################################
add_executable(server server.cpp)
target_include_directories(
        server
        PUBLIC
        net
)
target_link_libraries(server  PRIVATE 
         net 
        ${OpenCV_LIBS}
        Eigen3::Eigen  
)


add_executable(request_reply request_reply.cpp)
target_include_directories(
        request_reply
        PUBLIC
        net
        ${OpenCV_LIBS}
        Eigen3::Eigen
)
target_link_libraries(request_reply  
        PRIVATE 
            net         
            ${OpenCV_LIBS}
            Eigen3::Eigen
)
################################################################