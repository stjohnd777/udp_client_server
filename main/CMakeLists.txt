project(mains)

add_executable(udp_send_rcv_example my_udp_send_rcv_example.cpp)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        udp_lib
)

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        udp_lib
)

add_executable(asio_example_tcp_sync asio_example_tcp_sync.cpp)

target_include_directories(
        example
        PUBLIC
        udp_lib
)

target_link_libraries(
        example
        PUBLIC
        udp_lib
)



#add_executable(main_tcp main_tcp.cpp)
#
#target_include_directories(
#        main_tcp
#        PUBLIC
#        udp_lib
#)
#
#target_link_libraries(
#        main_tcp
#        PUBLIC
#        udp_lib
#)