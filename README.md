# Phlegmatic - Top-Down Action RPG

![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Raylib](https://img.shields.io/badge/Engine-Raylib%20v6.2-white?style=for-the-badge&logo=raylib&logoColor=black&color=white)
![CMake](https://img.shields.io/badge/Build-CMake%203.20+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![GCC](https://img.shields.io/badge/Compiler-GCC%2016.2.0%20(MinGW--w64)-blue?style=for-the-badge&logo=gnu&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20(x64)-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

A high-performance C++ top-down action RPG environment built with **Raylib 6.2** and **MinGW-w64 (GCC 16)**. Features telegraphed enemy attack cones/circles/lines, active timing-based parrying, dynamic camera tracking, particle feedback, procedural sound synthesis, and a modular Storyboard narrative system.

---

## Game Concept & Design Philosophy

### The "Phlegmatic" Ethos
In classical temperament theory, the **phlegmatic** disposition denotes a calm, composed, unemotional demeanor under extreme pressure. 

In *Phlegmatic*, this philosophy directly drives the core combat loop. Unlike conventional button-mashing hack-and-slash games where players spam dodges and rapid clicks, *Phlegmatic* is built around **deliberate observation, spatial awareness, and composure**:

1. **Telegraphed Intent (No Cheap Hits)**:
   - Every enemy action is announced via bold, high-contrast geometric ground telegraphs (cones, rings, directional beams).
   - Telegraphs fill dynamically from 0% to 100%, communicating the exact millisecond of impact.
   - The battlefield is readable at all times, making every hit taken a lesson in positioning rather than unpredictable RNG.

2. **The Art of the Parry (Composure Over Panic)**:
   - Standing inside an enemy's active telegraph is terrifying—yet that is precisely where the greatest rewards lie.
   - Pressing **`[Q]`** activates a precise parry window. Rather than running away, standing your ground and deflecting the strike at the apex of its charge completely nullifies incoming damage, plays a resonant metallic chime, and shatters the enemy's stance (`STAGGERED! / DEFENSE BROKEN!`).
   - Successful parries grant an immediate **+50% Counter-Attack Damage Buff**, incentivizing mastery over avoidance.

3. **Tactical Attack Cadence**:
   - **Light Attacks (`L-Click`)**: Agile, low-stamina slashes designed for poking, testing enemy poise, and swift recovery.
   - **Heavy Attacks (`R-Click`)**: Devastating lunging cleaves with significant windup. High risk, but deals massive damage and huge knockback, especially against staggered foes.

4. **Storyboard-Driven Progression**:
   - Designed to tell an episodic narrative through combat trials and arena encounters.
   - Built-in Storyboard System with an interactive overlay (**`[TAB]`** or **`[F1]`**) to track narrative context, trial objectives, and combat directives as you progress through each chapter.

5. **Asset-Agnostic Modular Architecture**:
   - All hitboxes, combat vectors, and collision calculations are decoupled from visual rendering.
   - The game ships with sleek, stylized procedural vector silhouettes and particle effects, ready to seamlessly transition to custom sprite sheets and tilesets whenever you drop them in.

---

## Combat Controls

| Action | Key / Input | Gameplay Role |
| :--- | :--- | :--- |
| **Movement** | `W, A, S, D` or `Arrow Keys` | 8-directional smooth movement with environment collision. |
| **Aiming** | `Mouse Cursor` | Character faces and aims weapon/attacks towards the reticle. |
| **Light Attack** | `Left Mouse Button` | Quick curved slash (110° arc). Low stamina cost, fast recovery. |
| **Heavy Attack** | `Right Mouse Button` | Charged cleave/thrust (140° arc, 115px range). Heavy damage & knockback. |
| **Parry** | `Q Key` (Timely Window) | Enters a 0.28s defensive stance. Timing it with an incoming strike deflects damage, stuns the attacker, and grants +50% counter damage. |
| **Storyboard Overlay** | `[TAB]` or `[F1]` | Toggles the Storyboard Modal to inspect chapters, lore notes, and combat trials. Cycle acts with `[LEFT]` / `[RIGHT]` / `[E]`. |
| **Restart / Respawn** | `[R] Key` | Instantly respawns player and resets the current combat wave. |

---

## Enemy Telegraph Archetypes

Enemies visually project their upcoming attacks on the arena floor before striking:

| Enemy Type | Telegraph Shape | Attack Nature | Counter Strategy |
| :--- | :--- | :--- | :--- |
| **Slasher Sentinel** | **Amber/Red Cone** (95° sweep, 115px) | Swift horizontal blade cleave. | Sidestep behind the enemy or tap `[Q]` just as the cone reaches 100% fill. |
| **Brute Crusher** | **Pulsing Circle** (150px AoE ring) | Heavy ground slam with lingering shockwave. | Dash outside the expanding perimeter or time a parry to break its massive poise. |
| **Vanguard Charger** | **Linear Beam** (50px width, 240px length) | High-velocity forward dash thrust. | Step perpendicularly out of the path, or parry head-on to interrupt the charge. |

---

## Project Structure

```
Phlegmatic/
├── .gitignore              # Configured for GitHub (ignores binaries, keeps vendor libs)
├── LICENSE                 # MIT License
├── CMakeLists.txt          # Cross-platform CMake configuration
├── build.bat               # One-click Windows compilation script
├── run.bat                 # Launches bin/phlegmatic.exe
├── assets/                 # Drop custom PNG assets here (see assets/README.md)
│   └── README.md
├── vendor/
│   └── raylib/             # Raylib 6.2 headers and static libraries (MinGW-w64)
│       ├── include/
│       └── lib/
└── src/
    ├── main.cpp            # Application entry point
    ├── Game.hpp / .cpp     # Game loop, camera lerp, wave manager, HUD
    ├── Player.hpp / .cpp   # Movement physics, attack arcs, parry timing, stamina
    ├── Enemy.hpp / .cpp    # State machine, telegraph geometry, stagger reaction
    ├── Combat.hpp          # AttackInfo, TelegraphIndicator definitions
    ├── Map.hpp / .cpp      # Arena bounds, stone pillars, collision, ambient lighting
    ├── Particles.hpp / .cpp# Screen shake, floating damage text, sparks, slash trails
    ├── SoundFX.hpp / .cpp  # Procedural sound synthesis (clangs, swooshes, alerts)
    ├── AssetManager.hpp/.cpp# Custom texture loader + procedural vector silhouettes
    ├── Storyboard.hpp / .cpp# Narrative acts, trial tracker, and [TAB] modal
    └── Config.hpp          # Game balancing constants and tuning parameters
```

---

## How to Build & Run

### Quick Windows Run:
Simply double-click or run:
```powershell
.\run.bat
```
*(If the binary has not yet been built, `run.bat` automatically invokes `build.bat` first).*

### Direct Build:
```powershell
.\build.bat
```
Produces an optimized, standalone binary at `bin\phlegmatic.exe` with static standard libraries.

### CMake Build (Ninja / Make):
```powershell
cmake -B build -G Ninja
cmake --build build
.\build\phlegmatic.exe
```

---

## Integrating Custom Assets

The engine is built so you can plug in your own artwork at any time without touching core game logic:
1. Place your PNG files in the [`assets/`](file:///c:/Users/Erick/Phlegmatic/assets/) directory:
   - `assets/player.png` - Character sprite / spritesheet
   - `assets/enemy_melee.png` - Slasher enemy sprite
   - `assets/enemy_brute.png` - Brute crusher boss sprite
   - `assets/tileset.png` - Arena floor tileset
2. [`AssetManager`](file:///c:/Users/Erick/Phlegmatic/src/AssetManager.cpp) automatically detects and binds them on startup.
3. If any asset is absent, the engine gracefully falls back to its procedural silhouette renderers.

---

## License

This project is licensed under the [MIT License](LICENSE) - see the [`LICENSE`](file:///c:/Users/Erick/Phlegmatic/LICENSE) file for details.

### Third-Party Libraries
- **[Raylib](https://www.raylib.com/)**: Licensed under the zlib/libpng license. Copyright (c) Ramon Santamaria (@raysan5).
