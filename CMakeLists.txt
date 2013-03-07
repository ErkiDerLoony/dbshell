project(dbshell C CXX)
cmake_minimum_required(VERSION 2.6)
add_definitions(--std=c++11 -Wall -g -O2)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Pqxx REQUIRED)
include_directories(${Pqxx_INCLUDE_DIRS})

find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})

add_subdirectory(src)
