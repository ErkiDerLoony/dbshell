file(GLOB SRC *.cpp)

add_executable(dbshell ${SRC})
target_link_libraries(dbshell ${PostgreSQL_LIBRARIES} ${Readline_LIBRARIES} ${Libiodbc_LIBRARIES})

add_library(table STATIC table.cpp)

install(TARGETS dbshell DESTINATION bin)
