project(udp_client)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Eigen3 CONFIG REQUIRED)


add_executable(
        udp_client
        udp_client.cpp
        )

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        net
        )

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        net
        ${OpenCV_LIBS}
        Eigen3::Eigen
)

