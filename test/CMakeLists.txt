add_executable(test_table table.cpp)
target_link_libraries(test_table table)

install(TARGETS test_table DESTINATION bin)
