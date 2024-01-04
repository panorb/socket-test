cmake_minimum_required(VERSION 3.28.1)

# Create projects
project(SocketTest LANGUAGES CXX)
# Find dependencies (managed via vcpkg)
find_package(catch2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(sdl2 CONFIG REQUIRED)

find_package(SFML COMPONENTS system graphics window CONFIG REQUIRED)

# Set CMake defaults
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")
list(REMOVE_ITEM SOURCE_FILES ${TEST_FILES})

# Add all the core files to a library
add_library(SocketCore STATIC ${HEADER_FILES} ${SOURCE_FILES})

if(WIN32)
  target_link_libraries(SocketCore PUBLIC wsock32 ws2_32)
endif()

target_link_libraries(SocketCore PUBLIC spdlog::spdlog)
target_link_libraries(SocketCore PRIVATE sfml-system sfml-graphics sfml-window)

# Add main entry point executable
add_executable(SocketMain main.cxx)
target_link_libraries(SocketMain PRIVATE SocketCore)

# Add test entry point executable
add_executable(SocketTest ${TEST_FILES})
target_link_libraries(SocketTest PRIVATE SocketCore Catch2::Catch2WithMain)

enable_testing()
include(Catch)
catch_discover_tests(SocketTest)

# Set debug arguments here, so that I don't have to manually set them again
# every time I make changes to the Cmake-Files
if(MSVC)
#	set_target_properties(SocketHelloWorld PROPERTIES
#							VS_DEBUGGER_WORKING_DIRECTORY "$(TargetDir)")
endif()
