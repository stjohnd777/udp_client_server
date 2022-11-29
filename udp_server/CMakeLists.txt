project(upd_server)

include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Eigen3 CONFIG REQUIRED)

add_executable(
        ${PROJECT_NAME}
        udp_server.cpp
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
