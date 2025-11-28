cmake_minimum_required(VERSION 3.20)

project(SeaFoxAdventures
    VERSION 1.0
    DESCRIPTION "A thrilling underwater adventure game"
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_executable(SeaFoxAdventures)

target_sources(SeaFoxAdventures PRIVATE
    src/main.cpp
)

target_compile_features(SeaFoxAdventures PRIVATE cxx_std_23)

if (MSVC)
    target_compile_options(SeaFoxAdventures PRIVATE /W4 /permissive-)
else()
    target_compile_options(SeaFoxAdventures PRIVATE -Wall -Wextra -Wpedantic)
endif()

set(PLATFORM "Desktop" CACHE STRING "" FORCE)
set(OPENGL_VERSION "3.3" CACHE STRING "" FORCE)
add_subdirectory(third_party/raylib)
target_link_libraries(SeaFoxAdventures PRIVATE raylib)