cmake_minimum_required(VERSION 3.18)

project(liona_term LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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
    sources/MainApplication.cpp
    sources/MainWindow.cpp
    sources/LionaTab.cpp
    sources/LionaTerminal.cpp
    sources/LionaFileExplorer.cpp
    headers/MainApplication.h
    headers/MainWindow.h
    headers/LionaTab.h
    headers/LionaTerminal.h
    headers/LionaFileExplorer.h
    resources/resources.qrc
)

target_include_directories(liona_term
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(liona_term
    PRIVATE
        Qt6::Widgets
        qtermwidget6
)
