set(game_sources
  game_main.cpp)

add_executable(SocketGame ${game_sources})

target_link_libraries(SocketGame PRIVATE SocketCore)

set(test_sources
  networking.test.cpp)

