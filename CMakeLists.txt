cmake_minimum_required(VERSION 3.16)

project(liona_term LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(liona_term
    main.cpp
)

target_link_libraries(liona_term
    PRIVATE
    Qt6::Widgets
)
