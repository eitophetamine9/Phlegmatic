<div align="center">

# PHLEGMATIC

### A Dark Fantasy Medieval Top-Down Action RPG

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Raylib](https://img.shields.io/badge/Engine-Raylib%20v6.2-white?style=for-the-badge&logo=raylib&logoColor=black&color=white)](https://www.raylib.com/)
[![CMake](https://img.shields.io/badge/Build-CMake%203.20+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![GCC](https://img.shields.io/badge/Compiler-GCC%2016.2.0%20(MinGW--w64)-blue?style=for-the-badge&logo=gnu&logoColor=white)](https://www.gnu.org/software/gcc/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20(x64)-0078D6?style=for-the-badge&logo=windows&logoColor=white)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](LICENSE)

*Explore the land of Tydeshire, learn it's deepest secrets and become its next savior from the madness...*

---

> **Comprehensive Game Documentation & Technical Reference**  
> Looking for deep combat mathematics, full lore codex, weapon attributes, bestiary AI patterns, and engine architecture?  
> Please consult **[DOCUMENTATION.md](documentation.md)**.

---

</div>

## Lore Briefing: Land of Tydeshire

> *“For over 500 years, the Land of Tydeshire was consumed by unending war until King Charlz unified the realm, crowning himself the Liege of Kings. He divided the continent into five sovereign realms to preserve peace: Zyphden, Azshden, Fleurshden, Noirden, and Solsden.*
>
> *Yet peace in Tydeshire was only ever borrowed. Three decades later, the Lord Liege vanished into madness, plunging the realm into paranoia and blood. As the ancient warding seals fracture, the true history of his fall lies scattered across forgotten steles and sunlit ruins, waiting for one who can pierce the illusions.”*

---

## Core Controls

| Action | Key / Input | Role |
| :--- | :--- | :--- |
| **Movement** | `W, A, S, D` or `Arrow Keys` | 8-directional fluid traversal with solid obstacle collision. |
| **Aiming** | `Mouse Cursor` | Reticle targeting for sword thrusts, dashes, and parry reflections. |
| **Light Attack** | `Left Mouse Button` | Swift slash/thrust with low stamina cost and quick recovery cadence. |
| **Heavy Cleave** | `Right Mouse Button` | High-damage signature moves (multi-stage cleaves, lunges, shockwaves). |
| **Dash (Evade)** | `Q Key` | Fast directional burst with **0.25s i-frames** (`EVADED`). |
| **Timed Parry** | `F Key` | Active guard window. Staggers melee foes and **reflects incoming projectiles**. |
| **Phlegmatic Trance** | `SPACE Key` | Active at 100 Composure. **5.0s invulnerability, zero stamina cost & auto-deflection aura**. |
| **Health Potion** | `1 Key` or `H Key` | Consumes 1 elixir to heal **+45 HP** immediately (**7.0s cooldown**). |
| **Interact / Advance** | `E Key` / `Enter` | Talk to NPCs, open chests, read monoliths, and transition doors. |
| **Dialogue Page** | `A Key` / `D Key` | Navigate between previous and next pages of active dialogue. |
| **Dismiss / Close** | `ESC Key` | Safely close active dialogue boxes, popups, and inventory modals. |
| **Objectives Tracker** | `O Key` | Toggle the pinned Chapter 1 quest objectives card. |
| **Lore & Equipment** | `I Key` or `TAB` | Open the in-game Journal, Equipment stats, and Totem slots. |
| **Interactive Realm Map** | `M Key` | Open the full parchment map with live player tracking and POI tooltips. |
| **Fullscreen Toggle** | `F11` or `Alt + Enter` | Switch between windowed and native crisp fullscreen display modes. |
| **Controls Guide** | `C Key` | Toggle the in-game keybindings overlay modal. |
| **Title Screen** | `~ Key` (Tilde / Grave) | Instantly return to the Title Screen from any state. |
| **Restart / Respawn** | `R Key` | Instantly respawn after defeat or restart the trial. |

---

## Current Version: 0.0.1d

> [!NOTE]
> Phlegmatic is currently on **Version 0.0.1d**.
>
> ### Version 0.0.1d Release Highlights
> * **Modular InputRouter & Centralized Hotkeys**: Fixed dropped hotkey inputs (\[O]\, \[M]\, \[I]\, \[ESC]\) and enabled direct cross-modal switching without key consumption collisions.
> * **Royal Halberd Rebalance & 3-Stage Heavy Combo**: Rebalanced damage (16 base light / 28 base heavy) and added a 3-stage heavy combo: Stage 1 forward lunge with i-frames, Stage 2 wide 220° sweep cleave, and Stage 3 flurry of 3 consecutive rapid stabs dealing 30% weapon damage each.
> * **Tactical Gear & Inventory Screen Overhaul (Image 1)**: Redesigned inventory with top tabs (\QUESTS | GEAR | INVENTORY | ABILITIES\), real-time attributes strip, 3-column tactical layout with wireframe armaments and apparel, interactive pedestal doll, and title badge.
> * **Tactical Frosted Realm Map Overhaul (Image 2)**: Redesigned interactive map with frosted teal glass styling, Cartesian grid, diamond numbered stage waypoints \<1>\–\<5>\ with active pulsing beacon, left stage inspection card, and bottom-right \[RESUME JOURNEY]\ button.
> 
> For detailed release notes, system mechanics breakdowns, math formulas, and lore codices, please consult **[DOCUMENTATION.md](documentation.md)**.

---

## Roadmap: Planned for Version 0.0.1d

> [!NOTE]
> Upcoming technical enhancements, Act IV northern realm expansion (Azshden and Fleurshden), dynamic environmental weather physics, and synthesized ambient soundscapes are documented in **[Version 0.0.1d Technical Roadmap in DOCUMENTATION.md](documentation.md#9-roadmap--version-001d-technical-plans)**.

---

## Setting Up & Running

Standalone precompiled 64-bit Windows executables are included in the repository:

### 1. Running the Game
* **Option 1**: Double-click [`phlegmatic.exe`](phlegmatic.exe) in the project root.
* **Option 2**: Run the launcher script:
  ```powershell
  .\run.bat
  ```
* **Option 3**: Launch from the bin folder:
  ```powershell
  .\bin\phlegmatic.exe
  ```

### 2. Building from Source

#### Automated Batch Build (MinGW-w64 GCC)
Compiles all C++ source files with `-O2 -static -std=c++17` and Raylib 6.2, outputting both `phlegmatic.exe` and `bin\phlegmatic.exe`:
```powershell
.\build.bat
```

#### CMake Build
```powershell
cmake -B build -G Ninja
cmake --build build
.\build\phlegmatic.exe
```

---

## Project Structure

```text
Phlegmatic/
├── phlegmatic.exe              # Standalone 64-bit Windows executable in project root
├── bin/
│   └── phlegmatic.exe          # Standalone executable in bin output
├── build.bat                   # Automated MinGW-w64 compilation script
├── run.bat                     # Quick launcher script
├── documentation.md            # Comprehensive game design & technical reference
├── CMakeLists.txt              # Cross-platform CMake configuration
├── assets/                     # Custom artwork & transition illustration drop
├── vendor/
│   └── raylib/                 # Precompiled Raylib 6.2 headers & static libraries
└── src/
    ├── main.cpp                # Game entry point & window lifecycle
    ├── core/                   # Engine config, procedural acoustic sound, particles
    ├── combat/                 # Weapons, armor, collision, math formulas, projectiles
    ├── world/                  # Level geometry, map environments, monoliths, chests
    ├── entities/               # Player, enemy AI, modular vector renderers
    ├── rendering/              # Procedural vector silhouettes & asset loader
    └── states/                 # State coordinators, handlers, and modular HUD/UI
```

---

## License

This project is licensed under the [MIT License](LICENSE) - see the [`LICENSE`](LICENSE) file for details.  
Third-party library [Raylib](https://www.raylib.com/) is licensed under the zlib/libpng license.
