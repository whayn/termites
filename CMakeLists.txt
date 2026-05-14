cmake_minimum_required(VERSION 3.14)
project(Termites VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# Raylib
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/raylib)

# Doctest
set(DOCTEST_WITH_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/doctest)

# ImGui && rlImgui
add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/rlImGui/rlImGui.cpp
)
target_include_directories(imgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/imgui>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/rlImGui>
)
target_link_libraries(imgui PUBLIC raylib)

# Core
add_library(termites_core STATIC
    src/Coord.cpp
    src/Grille.cpp
    src/Termite.cpp
    src/Jeu.cpp
    src/Application.cpp
    src/Colonie.cpp
    src/TermiteVisuel.cpp
    src/GestionnaireSprites.cpp
)

target_include_directories(termites_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(termites_core PUBLIC imgui raylib doctest::doctest)

target_compile_options(termites_core PRIVATE -Wall -Wextra)

# Executables
# GUI
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE termites_core)

# CLI
add_executable(main-cli src/main_cli.cpp)
target_link_libraries(main-cli PRIVATE termites_core)

# Tests
add_executable(tests src/test.cpp)
target_link_libraries(tests PRIVATE termites_core doctest::doctest)

# Make tests discoverable by CTest
include(CTest)
enable_testing()
add_test(NAME TermitesTests COMMAND tests)
