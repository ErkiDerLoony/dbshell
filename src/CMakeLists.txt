file(GLOB SRC *.cpp)

add_executable(dbshell ${SRC})
target_link_libraries(dbshell ${PostgreSQL_LIBRARIES} ${Readline_LIBRARIES} ${Libiodbc_LIBRARIES})

add_library(table STATIC table.cpp)
add_library(prefixes STATIC prefixes.cpp)

install(TARGETS dbshell DESTINATION bin)
