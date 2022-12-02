project(mains)
set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 CONFIG REQUIRED)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

################################################################
# Process to Process Procucer Consumer
################################################################
add_executable(producer producer.cpp)
target_include_directories( producer PUBLIC net )
target_link_libraries(producer  PRIVATE net )


add_executable(consumer consumer.cpp)
target_include_directories( consumer PUBLIC  net)
target_link_libraries(consumer  PRIVATE net )
################################################################


################################################################
# Procees:Client Request/Reply Process:Server
################################################################
add_executable(server server.cpp)
target_include_directories(server PUBLIC net)
target_link_libraries(server  PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen )


add_executable(client client.cpp)
target_include_directories( client PUBLIC net ${OpenCV_LIBS} Eigen3::Eigen )
target_link_libraries(client PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen)
################################################################



################################################################
# 
################################################################
add_executable(server_struct server_struct.cpp)
target_include_directories(server_struct PUBLIC net)
target_link_libraries(server_struct  PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen )


add_executable(client_struct client_struct)
target_include_directories( client_struct PUBLIC net ${OpenCV_LIBS} Eigen3::Eigen )
target_link_libraries(client_struct PRIVATE net ${OpenCV_LIBS} Eigen3::Eigen)
################################################################