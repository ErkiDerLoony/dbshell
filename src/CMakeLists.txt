file(GLOB SRC *.cpp)

add_executable(dbshell ${SRC})
target_link_libraries(dbshell ${PostgreSQL_LIBRARIES} ${Readline_LIBRARIES} ${Libiodbc_LIBRARIES})

add_library(log STATIC log.cpp)

install(TARGETS dbshell DESTINATION bin)
