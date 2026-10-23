cmake_minimum_required(VERSION 3.20)
project(Phlegmatic LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/core/SoundFX.cpp
    src/core/Particles.cpp
    src/world/Map.cpp
    src/world/WaveManager.cpp
    src/rendering/AssetManager.cpp
    src/entities/Player.cpp
    src/entities/PlayerRenderer.cpp
    src/entities/Enemy.cpp
    src/entities/EnemyRenderer.cpp
    src/combat/Projectile.cpp
    src/combat/Inventory.cpp
    src/combat/CollisionSystem.cpp
    src/states/TitleUI.cpp
    src/states/HUD.cpp
    src/states/ModalUI.cpp
    src/states/UI.cpp
    src/states/Cutscene.cpp
    src/states/Tutorial.cpp
    src/states/RuinedCastleHandler.cpp
    src/states/Game.cpp
)

add_executable(phlegmatic ${SOURCES})

target_include_directories(phlegmatic PRIVATE
    src
    src/core
    src/combat
    src/world
    src/entities
    src/rendering
    src/states
    vendor/raylib/include
)

target_link_directories(phlegmatic PRIVATE
    vendor/raylib/lib
)

target_link_libraries(phlegmatic PRIVATE
    raylib
    opengl32
    gdi32
    winmm
    -static
)
