cmake_minimum_required(VERSION 3.10)
project(lm_upd)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(net)
add_subdirectory(udp_server)
add_subdirectory(udp_client)
add_subdirectory(test)
add_subdirectory(main)

