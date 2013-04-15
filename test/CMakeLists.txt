add_executable(test_log log.cpp)
target_link_libraries(test_log log)

install(TARGETS test_log DESTINATION bin)
