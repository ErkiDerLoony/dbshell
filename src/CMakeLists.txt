file(GLOB SRC *.cpp)

add_executable(dbshell ${SRC})
target_link_libraries(dbshell ${PostgreSQL_LIBRARIES} ${Pqxx_LIBRARIES})

install(TARGETS dbshell DESTINATION bin)
