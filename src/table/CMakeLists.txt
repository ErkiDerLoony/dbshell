file(GLOB SRC *.cpp)

add_library(table STATIC ${SRC})
add_executable(table_test ${SRC})

install(TARGETS table_test DESTINATION bin)
