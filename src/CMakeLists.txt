add_subdirectory(table)

file(GLOB SRC *.cpp)

add_executable(dbshell ${SRC})
target_link_libraries(dbshell ${PostgreSQL_LIBRARIES} ${Readline_LIBRARIES} ${Libiodbc_LIBRARIES} table)

install(TARGETS dbshell DESTINATION bin)
