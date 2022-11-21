cmake_minimum_required(VERSION 3.23)

project(udp_client)

add_executable(
        udp_client
        udp_client.cpp
        )

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        udp_lib
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        udp_lib
        )
