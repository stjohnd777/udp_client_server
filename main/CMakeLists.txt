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


add_executable(book_example book_example.cpp)
target_include_directories(
        book_example
        PUBLIC
        net
)
target_link_libraries(book_example  PRIVATE net )