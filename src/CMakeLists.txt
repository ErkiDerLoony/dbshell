file(GLOB SRC *.cpp)

add_executable(dbshell ${SRC})
target_link_libraries(dbshell ${PostgreSQL_LIBRARIES} ${Readline_LIBRARIES} ${Libiodbc_LIBRARIES})

add_library(table STATIC EXCLUDE_FROM_ALL table.cpp)
add_library(prefixes STATIC EXCLUDE_FROM_ALL prefixes.cpp)

install(TARGETS dbshell DESTINATION bin)
