set(networking_headers
  networking.hpp
  socket_address.hpp
  socket_util.hpp
  tcp_socket.hpp
  udp_socket.hpp)

set(networking_sources
  socket_address.cpp
  socket_util.cpp
  tcp_socket.cpp
  udp_socket.cpp)

set(main_headers
  game_core.hpp)

set(main_sources
  game_core.cpp)

source_group("Header Files/Networking" FILES ${networking_headers})
source_group("Source Files/Networking" FILES ${networking_sources})

source_group("Header Files/Main" FILES ${main_headers})
source_group("Source Files/Main" FILES ${main_sources})

add_library(SocketCore STATIC
  ${networking_headers}
  ${networking_sources}
  ${main_headers}
  ${main_sources})

if (WIN32)
  target_link_libraries(SocketCore PUBLIC wsock32 ws2_32)
endif()

target_link_libraries(SocketCore PUBLIC spdlog::spdlog)
target_link_libraries(SocketCore PRIVATE sfml-system sfml-graphics sfml-window)

