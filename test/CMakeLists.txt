
add_executable(test_log test_log.cc)

target_link_libraries(test_log PRIVATE log pthread)

add_executable(test_eventloop test_eventloop.cc)
target_link_libraries(test_eventloop PRIVATE log)