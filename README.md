# Phlegmatic - Top-Down Action RPG

![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Raylib](https://img.shields.io/badge/Engine-Raylib%20v6.2-white?style=for-the-badge&logo=raylib&logoColor=black&color=white)
![CMake](https://img.shields.io/badge/Build-CMake%203.20+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![GCC](https://img.shields.io/badge/Compiler-GCC%2016.2.0%20(MinGW--w64)-blue?style=for-the-badge&logo=gnu&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20(x64)-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

A fast-paced, skill-based C++ top-down action RPG environment built with **Raylib 6.2** and **MinGW-w64 (GCC 16)**. Features deliberate observation, spatial awareness, active timing-based defense, an interactive armory hub with 3 weapons, clear enemy attack telegraphs, real-time projectile physics, and a multi-phase boss encounter.

---

## Key Features

* **Title Screen GUI**: Dark fantasy start screen with an animated **PLAY GAME** button and direct access to the **CONTROLS** modal.
* **Armory Sanctuary (Lobby Hub)**:
  * Safe sanctuary to experiment with movement, attacks, and parry timings before entering battle.
  * **3 Weapon Pedestals**: Switch active weapon at any time with `[E]` or mouse click.
  * **Training Dummy**: Test light/heavy attacks, damage numbers, and poise stagger with zero risk.
  * **Arena Portal**: Step into the glowing northern ring or press `[E]` to begin arena trials.
* **Precision Combat System**:
  * **`Q` Key — Dash**: Directional roll/burst with **invulnerability frames (i-frames)** that harmlessly phases through melee sweeps and projectiles (`EVADED`).
  * **`F` Key — Timed Parry**: Active 0.28s guard window with radiant cyan-gold barrier. Deflecting melee strikes **staggers attackers (`DEFENSE BROKEN!`) for 2.0s** and grants an immediate **+50% Counter-Attack Damage Buff**.
  * **Projectile Parry Reflection**: Parrying enemy spells and arrows deflects them directly back toward your cursor with amplified counter damage.
* **In-Game Controls Overlay**: Press **`[C]`** or **`[TAB]`** at any time (in Title, Lobby, or Arena) to open an interactive glassmorphic controls guide.
* **Geometric Attack Telegraphs**: Enemies announce all upcoming attacks on the arena floor (cones, lines, circles) that fill dynamically from 0% to 100% before striking.
* **Multi-Phase Boss Encounter**: **The Grand Inquisitor** (500 HP) featuring a dedicated boss health bar and three distinct combat phases (12-way Projectile Nova, focused Arcane Beam, and Teleport Ground Smite).

---

## Combat Controls

| Action | Key / Input | Gameplay Role |
| :--- | :--- | :--- |
| **Movement** | `W, A, S, D` or `Arrow Keys` | 8-directional smooth movement with map wall and pillar collisions. |
| **Aiming** | `Mouse Cursor` | Reticle targeting for weapon swings, dashes, and parry deflections. |
| **Light Attack** | `Left Mouse Button` | Agile slash/thrust. Low stamina cost, swift recovery cadence. |
| **Heavy Attack** | `Right Mouse Button` | High-impact cleave/slam. High damage, enemy poise stagger, and knockback. |
| **Dash** | `Q Key` | Fast directional burst with **i-frames** (`EVADED`) to dodge sweeps and projectiles. |
| **Parry** | `F Key` | 0.28s defensive guard. Staggers melee foes for 2.0s and **reflects incoming projectiles**. |
| **Interact** | `E Key` / `Left Click` | Equip weapons at pedestals in the Lobby, or step into the Arena Portal. |
| **Controls Menu** | `C Key` or `TAB` | Toggle the in-game interactive controls guide at any time. |
| **Restart / Respawn** | `R Key` | Instantly respawn after defeat or restart the combat trial. |
| **Return to Lobby** | `L Key` or `ESC` | Leave the arena and return to the Armory Sanctuary. |

---

## Weapon Arsenal

Each weapon features custom attack arcs, damage profiles, windups, heavy cooldowns, and dash passives:

### 1. Broadsword (Balanced Stance)
* **Design**: Versatile blade with dependable range and fluid recovery.
* **Light Attack**: 25 damage, 75px reach, 110° arc, 0.22s duration.
* **Heavy Attack**: 65 damage, 115px reach, 140° sweeping cleave, heavy knockback (1.90s cooldown).
* **Dash**: 0.85s cooldown, 20 stamina.

### 2. Dual Daggers (Agility & Flurry)
* **Design**: Fast-cadence twin daggers built for hit-and-run evasion.
* **Light Attack**: 18 damage, 62px reach, rapid twin-slash cadence.
* **Heavy Attack**: 52 damage, 75px reach, 360° twin-blade whirlwind spin (1.20s cooldown).
* **Dash**: 0.50s cooldown, 14 stamina.

### 3. War Mace (Crushing Poise & Shockwaves)
* **Design**: Massive two-handed bludgeon designed to crush heavy enemy armor.
* **Light Attack**: 40 damage, 82px reach, high enemy poise damage.
* **Heavy Attack**: 95 damage, 135px reach, devastating earth-tremor ground slam (3.20s cooldown).
* **Dash**: 1.25s cooldown, 26 stamina.

---

## Enemies & Boss Encounters

| Enemy | Type | Telegraph Shape | Combat Behavior & Strategy |
| :--- | :--- | :--- | :--- |
| **Cultist Slasher** | Melee Vanguard | Amber/Red Cone (95°, 115px) | Aggressive forward blade slash. Sidestep or time `[F]` to stagger. |
| **Shadow Charger** | Dash Melee | Linear Path (50px width, 240px) | High-speed dash thrust. Step perpendicular to path or parry head-on. |
| **Iron Brute** | Heavy Crusher | Expanding Circle (150px AoE) | Tremor ground slam. Dash clear with `[Q]` or break poise with War Mace. |
| **Deathseeker Archer** | Ranged Kiting | Red Laser Line (380px range) | Backs away if approached; fires high-velocity piercing bolts. |
| **Void Pyromancer** | Occult Caster | Arcane Fan Sector (320px) | Fires a 3-way fan spread of void orbs. Evade with `[Q]` or reflect with `[F]`. |
| **The Grand Inquisitor** | **Grand Boss** | Multi-Phase Telegraphs | 500 HP boss with gold-rimmed top health bar. Rotates between **12-way Projectile Nova**, **3-shot Arcane Beam**, and **Teleport & Cataclysmic Smite**. |

---

## Quick Start & Running

Standalone precompiled binaries are included in the repository:

### Running the Executable:
* **Option 1**: Double-click [`phlegmatic.exe`](phlegmatic.exe) in the project root.
* **Option 2**: Run [`run.bat`](run.bat):
  ```powershell
  .\run.bat
  ```
* **Option 3**: Run [`bin/phlegmatic.exe`](bin/phlegmatic.exe):
  ```powershell
  .\bin\phlegmatic.exe
  ```

### Building from Source:
* **Batch Build (MinGW-w64 GCC)**:
  ```powershell
  .\build.bat
  ```
  *Compiles all source files with `-O2 -static` and outputs both `phlegmatic.exe` and `bin\phlegmatic.exe`.*

* **CMake Build**:
  ```powershell
  cmake -B build -G Ninja
  cmake --build build
  .\build\phlegmatic.exe
  ```

---

## Project Structure

```
Phlegmatic/
├── phlegmatic.exe              # Standalone executable in project root
├── bin/
│   └── phlegmatic.exe          # Standalone executable in bin/
├── build.bat                   # Automated MinGW-w64 compilation script
├── run.bat                     # Quick launcher script
├── CMakeLists.txt              # Cross-platform CMake configuration
├── vendor/
│   └── raylib/                 # Precompiled Raylib 6.2 headers & static libraries
└── src/
    ├── main.cpp                # Program entry point & window lifecycle
    │
    ├── core/                   # Engine audio, particle system, and configuration
    │   ├── Config.hpp          # Game constants, window size, weapon baseline stats
    │   ├── SoundFX.hpp / .cpp  # Procedural audio synthesis (clangs, swooshes, alerts)
    │   └── Particles.hpp / .cpp# Screen shake, floating damage text, spark bursts
    │
    ├── combat/                 # Weapons, combat math, and projectile engine
    │   ├── CombatTypes.hpp     # AttackInfo, TelegraphIndicator, enums
    │   ├── Weapon.hpp          # Sword, Daggers, and Mace definitions & scalings
    │   └── Projectile.hpp / .cpp# Real-time projectile physics, walls, parry reflection
    │
    ├── world/                  # Level geometry and interactive world objects
    │   ├── Map.hpp / .cpp      # Lobby sanctuary and combat arena layouts
    │   └── Interactable.hpp    # Weapon pedestals, training dummy, arena portal
    │
    ├── entities/               # Player character and enemy AI state machines
    │   ├── Player.hpp / .cpp   # WASD movement, Q Dash (i-frames), F Parry, attacks
    │   └── Enemy.hpp / .cpp    # 5 enemy archetypes + The Grand Inquisitor Boss
    │
    ├── rendering/              # Procedural silhouettes and asset loader
    │   └── AssetManager.hpp / .cpp# Sprite loader + procedural vector silhouettes
    │
    └── states/                 # State management and UI rendering
        ├── GameState.hpp       # State enums (TITLE_SCREEN, LOBBY, ARENA, etc.)
        ├── Game.hpp / .cpp     # Master game loop & combat collision coordinator
        └── UI.hpp / .cpp       # Title screen, Controls menu, Combat HUD, Boss bar
```

---

## Custom Assets Integration

The engine is asset-agnostic and ready for custom artwork:
1. Place custom PNG files into the [`assets/`](assets/) directory:
   - `assets/player.png` — Player character sprite
   - `assets/enemy_melee.png` — Slasher enemy sprite
   - `assets/tileset.png` — Arena floor tileset
2. `AssetManager` automatically detects and binds them on boot, replacing the built-in vector silhouettes.

---

## License

This project is licensed under the [MIT License](LICENSE) - see the [`LICENSE`](LICENSE) file for details.
Third-party library [Raylib](https://www.raylib.com/) is licensed under the zlib/libpng license.
