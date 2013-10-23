project(dbshell C CXX)
cmake_minimum_required(VERSION 2.6)
add_definitions(-std=c++0x -Wall -g -O2 -pedantic)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Readline REQUIRED)
include_directories(${Readline_INCLUDE_DIRS})

find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})

find_package(Libiodbc REQUIRED)
include_directories(${Libiodbc_INCLUDE_DIRS})

find_package(Doxygen)
if (DOXYGEN_FOUND)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE}
                    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    VERBATIM)
  set_property(DIRECTORY APPEND PROPERTY
               ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/doc)
endif (DOXYGEN_FOUND)

add_subdirectory(src)

include_directories(src)
add_subdirectory(test EXCLUDE_FROM_ALL)
