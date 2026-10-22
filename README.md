# Phlegmatic - Top-Down Action RPG

![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Raylib](https://img.shields.io/badge/Engine-Raylib%20v6.2-white?style=for-the-badge&logo=raylib&logoColor=black&color=white)
![CMake](https://img.shields.io/badge/Build-CMake%203.20+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![GCC](https://img.shields.io/badge/Compiler-GCC%2016.2.0%20(MinGW--w64)-blue?style=for-the-badge&logo=gnu&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20(x64)-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

A fast, skill-based C++ top-down action RPG environment built with **Raylib 6.2** and **MinGW-w64 (GCC 16)**. Features precision combat, directional dashing with i-frames, timed parries with projectile deflection, an interactive armory lobby with three weapon types, telegraphed enemy attack cones/beams, and a multi-phase projectile-firing boss encounter.

---

## Combat Controls

| Action | Key / Input | Gameplay Effect |
| :--- | :--- | :--- |
| **Movement** | `W, A, S, D` or `Arrow Keys` | 8-directional smooth movement with environment collision. |
| **Aiming** | `Mouse Reticle` | Character aims weapons, attacks, and parry deflections toward cursor. |
| **Light Attack** | `Left Mouse Button` | Swift slash. Low stamina cost, rapid recovery. |
| **Heavy Attack** | `Right Mouse Button` | Powerful cleave/slam. High damage, poise break, and heavy knockback. |
| **Dash** | `Q Key` | Fast directional burst with **invulnerability frames (i-frames)** (`EVADED`). |
| **Parry** | `F Key` | 0.28s guard window. Deflects melee strikes (**staggers attackers for 2.0s**) and **reflects enemy projectiles** back with +50% counter damage. |
| **Interact** | `E Key` / `Left Click` | Equip weapons at pedestals in the Lobby, or step into the Arena Portal. |
| **Controls Menu** | `C Key` or `TAB` | Toggle the in-game glassmorphic controls guide at any time. |
| **Restart / Respawn** | `R Key` | Instantly reset the current combat wave or respawn after defeat. |
| **Return to Lobby** | `L Key` or `ESC` | Return from the arena to the Armory Sanctuary. |

---

## Weapon Arsenal

Equip and test weapons at pedestals in the **Armory Sanctuary (Lobby)** or practice against the **Training Dummy**:

* **Broadsword (Balanced Stance)**:
  * Reliable reach and balanced cadence.
  * *Light*: 25 damage, 75 range, 110° arc.
  * *Heavy*: 65 damage, 115 range, 140° arc sweeping cleave.
* **Dual Daggers (Agility & Flurry)**:
  * Rapid dual-strike cadence with reduced stamina costs.
  * *Light*: 18 damage, 62 range fast slashes.
  * *Heavy*: 52 damage, 75 range 360° twin-blade whirlwind spin.
  * *Passive*: Dash stamina cost reduced to 12 (from 18), cooldown reduced to 0.42s (from 0.55s).
* **War Mace (Crushing Poise & Shockwaves)**:
  * Heavy bludgeon with maximum poise damage and knockback.
  * *Light*: 40 damage, 82 range bludgeon.
  * *Heavy*: 95 damage, 135 range earth-shattering ground slam.

---

## Enemies & Boss Encounters

Enemies announce all attacks with real-time filled geometric telegraphs:

| Enemy | Archetype | Attack Telegraph | Threat & Strategy |
| :--- | :--- | :--- | :--- |
| **Cultist Slasher** | Melee Vanguard | Amber/Red Cone (95°, 115px) | Swift horizontal blade slash. Sidestep or time `[F]` to stagger. |
| **Shadow Charger** | Dash Melee | Linear Path (50px width, 240px) | High-speed forward thrust. Step laterally or parry head-on. |
| **Iron Brute** | Heavy Crusher | Expanding Circle (150px AoE) | Tremor ground slam. Dash clear with `[Q]` or break poise. |
| **Deathseeker Archer** | Ranged Kiting | Red Laser Line (380px range) | Kites backward if approached. Fires piercing high-speed bolts. |
| **Void Pyromancer** | Occult Caster | Arcane Fan Sector (320px) | Fires a 3-way fan spread of void orbs. Dodge with `[Q]` or reflect with `[F]`. |
| **The Grand Inquisitor** | **Grand Boss** | Multi-Phase Telegraphs | 500 HP boss with dedicated top health bar. Rotates between **12-way Projectile Nova**, **3-shot Arcane Beam**, and **Teleport & Cataclysmic Smite**. |

---

## Quick Start & Running

A precompiled standalone executable is included in `bin/`:

### Run Directly:
* Simply launch `bin\phlegmatic.exe` or double-click `run.bat`:
  ```powershell
  .\run.bat
  ```

### Build from Source:
* **One-Click Batch Script**:
  ```powershell
  .\build.bat
  ```
* **CMake Build**:
  ```powershell
  cmake -B build -G Ninja
  cmake --build build
  .\build\phlegmatic.exe
  ```

---

## Modular Architecture

```
Phlegmatic/
├── bin/
│   └── phlegmatic.exe          # Standalone compiled executable (Windows x64)
├── build.bat                   # Compilation script
├── run.bat                     # Quick launcher
├── CMakeLists.txt              # Cross-platform CMake configuration
├── vendor/
│   └── raylib/                 # Raylib 6.2 headers & static libraries
└── src/
    ├── main.cpp                # Program entry point
    ├── core/                   # Audio synthesis, particles, and balance constants
    │   ├── Config.hpp
    │   ├── SoundFX.hpp / .cpp
    │   └── Particles.hpp / .cpp
    ├── combat/                 # Weapons, combat rules, and projectiles
    │   ├── CombatTypes.hpp
    │   ├── Weapon.hpp
    │   └── Projectile.hpp / .cpp
    ├── world/                  # Maps and interactables
    │   ├── Map.hpp / .cpp      # Lobby sanctuary & Arena maps
    │   └── Interactable.hpp    # Weapon pedestals, dummy, arena portal
    ├── entities/               # Player & AI state machines
    │   ├── Player.hpp / .cpp   # Q Dash, F Parry, attack states
    │   └── Enemy.hpp / .cpp    # 5 enemy archetypes + Inquisitor Boss
    ├── rendering/              # Custom asset management & procedural silhouettes
    │   └── AssetManager.hpp / .cpp
    └── states/                 # State management & user interfaces
        ├── GameState.hpp
        ├── Game.hpp / .cpp
        └── UI.hpp / .cpp       # Title screen, Controls menu, HUD, Boss bar
```

---

## Custom Assets Integration

Artwork can be dropped into [`assets/`](assets/) anytime:
* Place PNG textures such as `assets/player.png`, `assets/enemy_melee.png`, `assets/tileset.png`.
* `AssetManager` automatically detects and binds them on boot, seamlessly replacing procedural vector silhouettes.

---

## License

Licensed under the [MIT License](LICENSE). Third-party library [Raylib](https://www.raylib.com/) is licensed under the zlib/libpng license.
