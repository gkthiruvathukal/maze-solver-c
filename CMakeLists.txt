cmake_minimum_required(VERSION 2.6)

if(UNIX AND NOT APPLE)
  message(">> Linux")
  add_compile_options(-DWITH_LINUX)
else()
  message(">> non-Linux")
  add_compile_options(-DWITH_APPLE) 
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(maze maze.c)

