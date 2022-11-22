project(mains)

add_executable(udp_send_rcv_example my_udp_send_rcv_example.cpp)

target_include_directories(
        udp_send_rcv_example
        PUBLIC
        net
)

target_link_libraries(udp_send_rcv_example  PRIVATE net )
 

add_executable(sams_example sams_example.cpp)

target_include_directories(
        sams_example
        PUBLIC
        net
)

target_link_libraries(sams_example  PRIVATE net )
 