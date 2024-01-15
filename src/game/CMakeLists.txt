set(game_sources
  game_main.cpp)

add_executable(SocketGame ${game_sources})

target_link_libraries(SocketGame PRIVATE SocketCore)

# copy assets to target file dir after build
add_custom_command(TARGET SocketGame POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SocketGame>/assets)

