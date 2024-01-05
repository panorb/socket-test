set(test_sources
  networking.test.cpp)

add_executable(SocketTest ${test_sources})
target_link_libraries(SocketTest PRIVATE Catch2::Catch2WithMain SocketCore)

enable_testing()
include(Catch)
catch_discover_tests(SocketTest)

