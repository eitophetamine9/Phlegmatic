cmake_minimum_required(VERSION 3.20)
project(Phlegmatic LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Map.cpp
    src/Particles.cpp
    src/SoundFX.cpp
    src/AssetManager.cpp
    src/Storyboard.cpp
)

add_executable(phlegmatic ${SOURCES})

# Include headers
target_include_directories(phlegmatic PRIVATE
    src
    vendor/raylib/include
)

# Link Raylib and Windows system libraries
target_link_directories(phlegmatic PRIVATE
    vendor/raylib/lib
)

target_link_libraries(phlegmatic PRIVATE
    raylib
    opengl32
    gdi32
    winmm
    -static-libgcc
    -static-libstdc++
)
