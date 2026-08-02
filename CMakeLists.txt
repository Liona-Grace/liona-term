cmake_minimum_required(VERSION 3.18)

project(liona_term LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.6 REQUIRED COMPONENTS Widgets)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(UPDATE_TRANSLATIONS OFF CACHE BOOL "" FORCE)
set(QTERMWIDGET_BUILD_PYTHON_BINDING OFF CACHE BOOL "" FORCE)

add_subdirectory(
    third_party/qtermwidget
    ${CMAKE_BINARY_DIR}/qtermwidget
)

add_executable(liona_term
    main.cpp
)

target_link_libraries(liona_term
    PRIVATE
    Qt6::Widgets
    qtermwidget6
)

target_compile_definitions(liona_term
    PRIVATE
    LIONA_COLOR_SCHEME_DIR="${CMAKE_CURRENT_SOURCE_DIR}/third_party/qtermwidget/lib/color-schemes"
)
