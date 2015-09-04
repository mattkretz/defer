add_executable(test_defer test.cpp)
set_target_properties(test_defer PROPERTIES COMPILE_FLAGS "-std=c++11 -Wall")
add_test(NAME test_defer COMMAND test_defer)
