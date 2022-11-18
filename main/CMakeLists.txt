cmake_minimum_required(VERSION 3.23)

project(main)

add_executable(${PROJECT_NAME} UdpServerSync.cpp)

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

add_executable(example SyncTCPClient.cpp)

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



add_executable(main_tcp main_tcp.cpp)

target_include_directories(
        main_tcp
        PUBLIC
        udp_lib
)

target_link_libraries(
        main_tcp
        PUBLIC
        udp_lib
)