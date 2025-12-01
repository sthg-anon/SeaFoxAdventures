cmake_minimum_required(VERSION 3.20)

project(SeaFoxAdventures
    VERSION 0.0.1
    DESCRIPTION "A thrilling underwater adventure game"
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_executable(SeaFoxAdventures)

set(SFA_SOURCE_FILES
    src/assets/sprites/TestImage.cpp
    src/assets/sprites/Tiles.cpp
    src/main.cpp
    src/Renderer.cpp
    src/TileData.cpp
    src/World.cpp
    src/WorldGenerator.cpp

    include/assets/sprites/TestImage.hpp
    include/assets/sprites/Tiles.hpp
    include/Renderer.hpp
    include/TileData.hpp
    include/TileType.hpp
    include/World.hpp
    include/WorldGenerator.hpp
)

target_sources(SeaFoxAdventures PRIVATE
    ${SFA_SOURCE_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SFA_SOURCE_FILES})

target_include_directories(SeaFoxAdventures PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(SeaFoxAdventures PRIVATE cxx_std_23)

if (MSVC)
    target_compile_options(SeaFoxAdventures PRIVATE /W4 /permissive-)
else()
    target_compile_options(SeaFoxAdventures PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Dependency: Raylib
set(PLATFORM "Desktop" CACHE STRING "" FORCE)
set(OPENGL_VERSION "3.3" CACHE STRING "" FORCE)
add_subdirectory(third_party/raylib)

# Dependency: GSL
add_subdirectory(third_party/gsl)

# Dependency: ImGui
add_library(imgui)
target_sources(imgui PRIVATE
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui.cpp
)
target_include_directories(imgui PUBLIC third_party/imgui)

# Dependency: rlImGui
add_library(rlImGui)
target_sources(rlImGui PRIVATE
    third_party/rlImGui/rlImGui.cpp
)
target_include_directories(rlImGui PUBLIC third_party/rlImGui)
target_compile_definitions(rlImGui PUBLIC NO_FONT_AWESOME)
target_link_libraries(rlImGui PUBLIC imgui raylib)

# Linking
target_link_libraries(SeaFoxAdventures PRIVATE
    Microsoft.GSL::GSL
    rlImGui
)