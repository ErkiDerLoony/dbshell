add_executable(test_table table.cpp)
target_link_libraries(test_table table)

add_executable(test_prefixes prefixes.cpp)
target_link_libraries(test_prefixes table prefixes)

install(TARGETS test_table test_prefixes DESTINATION bin)
