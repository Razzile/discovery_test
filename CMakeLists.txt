cmake_minimum_required (VERSION 2.8.11)
project (discorvery CXX)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

add_executable(discovery_client client.cc)
add_executable(discovery_server server.cc)

if(WIN32)
  target_link_libraries(discovery_client wsock32 ws2_32)
  target_link_libraries(discovery_server wsock32 ws2_32)
endif()