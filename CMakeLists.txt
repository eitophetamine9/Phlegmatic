cmake_minimum_required(VERSION 3.20)
project(Phlegmatic LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/core/SoundFX.cpp
    src/core/Particles.cpp
    src/core/SaveManager.cpp
    src/world/TimeOfDay.cpp
    src/world/Map.cpp
    src/world/WaveManager.cpp
    src/rendering/AssetManager.cpp
    src/rendering/FontManager.cpp
    src/entities/Player.cpp
    src/entities/PlayerRenderer.cpp
    src/entities/Enemy.cpp
    src/entities/EnemyRenderer.cpp
    src/combat/Projectile.cpp
    src/combat/Inventory.cpp
    src/combat/CollisionSystem.cpp
    src/combat/LootDrop.cpp
    src/ui/TitleUI.cpp
    src/ui/HUD.cpp
    src/ui/ModalUI.cpp
    src/ui/UI.cpp
    src/states/Cutscene.cpp
    src/states/Tutorial.cpp
    src/states/realms/RuinedCastleHandler.cpp
    src/states/realms/CrossroadsHandler.cpp
    src/states/realms/ZyphdenHandler.cpp
    src/states/realms/QuarryHandler.cpp
    src/states/realms/RestlessMistForestHandler.cpp
    src/states/realms/NorthZyphdenHandler.cpp
    src/states/realms/FrostCavernsHandler.cpp
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
    src/states/realms
    src/ui
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
