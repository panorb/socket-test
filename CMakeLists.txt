cmake_minimum_required(VERSION 3.27.2)

# Create projects
project(SocketTest LANGUAGES CXX)
include_directories(src)

# Find dependencies (managed via vcpkg)
find_package(catch2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)

# Set CMake defaults
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(src)

# Set debug arguments here, so that I don't have to manually set them again
# every time I make changes to the Cmake-Files
if(MSVC)
	set(VS_STARTUP_PROJECT SocketTest)
#	set_target_properties(SocketHelloWorld PROPERTIES
#							VS_DEBUGGER_WORKING_DIRECTORY "$(TargetDir)")
endif()
