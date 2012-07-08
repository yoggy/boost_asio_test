cmake_minimum_required(VERSION 2.8)

PROJECT(tcp_client)
PROJECT(tcp_server_single)
PROJECT(tcp_server_multi)

if(WIN32)
  # nothing to do?
elseif(APPLE)
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
  set(CMAKE_CXX_FLAGS "-g -Wall")
  set(CMAKE_C_FLAGS "-g -Wall")
  set(LIBS boost_thread-mt boost_system-mt)
endif()

add_executable(tcp_client tcp_client.cpp)
target_link_libraries(tcp_client boost_thread-mt boost_system-mt)

add_executable(tcp_server_single tcp_server_single.cpp)
target_link_libraries(tcp_server_single ${LIBS})

add_executable(tcp_server_multi tcp_server_multi.cpp)
target_link_libraries(tcp_server_multi ${LIBS})

INSTALL(TARGETS tcp_client tcp_server_single tcp_server_multi RUNTIME DESTINATION bin)

