cmake_minimum_required(VERSION 2.84 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2")

project(monopoly)

include_directories(include)

add_executable(main src/main.cpp)

add_library(lib
        src/Player.cpp
        src/Computer.cpp
        src/Human.cpp
        src/Cell.cpp
        src/GameBoard.cpp
        src/Game.cpp
)

target_link_libraries(main lib)