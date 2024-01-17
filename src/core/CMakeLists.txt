file(DOWNLOAD
  https://raw.githubusercontent.com/eteran/cpp-utilities/master/fixed/include/cpp-utilities/fixed.h
  ${CMAKE_CURRENT_LIST_DIR}/fixed.h)

set(networking_headers
  networking.hpp
  socket_address.hpp
  socket_util.hpp
  tcp_socket.hpp
  udp_socket.hpp
  memory_stream.hpp)

set(networking_sources
  socket_address.cpp
  socket_util.cpp
  tcp_socket.cpp
  udp_socket.cpp
  memory_stream.cpp)

set(main_headers
  game_core.hpp
  game_object.hpp
  game_player.hpp)

set(main_sources
  game_core.cpp
  game_object.cpp
  game_player.cpp)

set(external_headers
  fixed.h)

source_group("Header Files/Networking" FILES ${networking_headers})
source_group("Source Files/Networking" FILES ${networking_sources})

source_group("Header Files/Main" FILES ${main_headers})
source_group("Source Files/Main" FILES ${main_sources})

source_group("Header Files/External" FILES ${external_headers})

add_library(SocketCore STATIC
  ${networking_headers}
  ${networking_sources}
  ${main_headers}
  ${main_sources}
  ${external_headers}
  )

if (WIN32)
  target_link_libraries(SocketCore PRIVATE wsock32 ws2_32 winmm.lib)
endif()

target_link_libraries(SocketCore PUBLIC spdlog::spdlog)
target_link_libraries(SocketCore PRIVATE raylib)

