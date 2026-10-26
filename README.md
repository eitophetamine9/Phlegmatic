<div align="center">

# PHLEGMATIC

### A Dark Fantasy Medieval Top-Down Action RPG

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Raylib](https://img.shields.io/badge/Engine-Raylib%20v6.2-white?style=for-the-badge&logo=raylib&logoColor=black&color=white)](https://www.raylib.com/)
[![CMake](https://img.shields.io/badge/Build-CMake%203.20+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![GCC](https://img.shields.io/badge/Compiler-GCC%2016.2.0%20(MinGW--w64)-blue?style=for-the-badge&logo=gnu&logoColor=white)](https://www.gnu.org/software/gcc/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20(x64)-0078D6?style=for-the-badge&logo=windows&logoColor=white)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](LICENSE)

*Explore the overgrown fortifications of Zyphden, master deliberate martial swordplay, uncover the fragmented history of King Charlz's madness, and pierce through the illusions of corrupted crusaders.*

---

> [!NOTE]
> 📖 **Comprehensive Game Documentation & Technical Reference**  
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
| **Health Potion** | `1 Key` or `H Key` | Consumes 1 elixir to heal **+45 HP** immediately (**7.0s cooldown**). |
| **Interact / Advance** | `E Key` / `Space` / `Enter` | Talk to NPCs, open chests, read monoliths, and transition doors. |
| **Dialogue Page** | `A Key` / `D Key` | Navigate between previous and next pages of active dialogue. |
| **Dismiss / Close** | `ESC Key` | Safely close active dialogue boxes, popups, and inventory modals. |
| **Objectives Tracker** | `O Key` | Toggle the pinned Chapter 1 quest objectives card. |
| **Lore & Equipment** | `I Key` or `TAB` | Open the in-game Journal, Equipment stats, and Totem slots. |
| **Controls Guide** | `C Key` | Toggle the in-game keybindings overlay modal. |
| **Title Screen** | `~ Key` (Tilde / Grave) | Instantly return to the Title Screen from any state. |
| **Restart / Respawn** | `R Key` | Instantly respawn after defeat or restart the trial. |

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
