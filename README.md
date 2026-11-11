<div align="center">

# PHLEGMATIC

### A Dark Fantasy Medieval Top-Down Action RPG

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Raylib](https://img.shields.io/badge/Engine-Raylib%20v6.2-white?style=for-the-badge&logo=raylib&logoColor=black&color=white)](https://www.raylib.com/)
[![CMake](https://img.shields.io/badge/Build-CMake%203.20+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![GCC](https://img.shields.io/badge/Compiler-GCC%2016.2.0%20(MinGW--w64)-blue?style=for-the-badge&logo=gnu&logoColor=white)](https://www.gnu.org/software/gcc/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20(x64)-0078D6?style=for-the-badge&logo=windows&logoColor=white)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](LICENSE)

*Explore the land of Tydeshire, learn its deepest secrets, and become its next savior from the madness...*

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
| **Aiming** | `Mouse Cursor` | Reticle targeting for weapon thrusts, dashes, and parry reflections. |
| **Light Attack** | `Left Mouse Button` | Swift slash/thrust combo (Daggers consume stamina per swing: `9`/`8`, Truth Seeker 2-hit combo, Twin Glacial Cleavers alternating slashes, Windrunner rapid Gale Arrows). |
| **Heavy Signature Combo** | `Right Mouse Button` | Multi-stage weapon combos (Daggers 3-stage, Mace 2-stage, Crusader Blade 3-stage, Truth Seeker 2-stage, Halberd 3-stage, Sun Claymore 2-stage Solar Cataclysm, Storm Glaive 2-stage Tempest Cyclone, Twin Glacial Cleavers 4-stage Fury of Cleaves). |
| **Armor Set Special Skill** | `V Key` | Triggers active skill on 3/3 matching armor sets (Dust Burst, Dawnfire Aegis, Seismic Quake, Shadow Decoy, Spike Wall, Basalt Spires, FROST NOVA). |
| **Dash (Evade)** | `Q Key` | Fast directional burst with **0.25s i-frames** (`EVADED`). |
| **Timed Parry** | `F Key` | Active guard window. Staggers melee foes, grants counter boost, and reflects projectiles toward cursor. |
| **Phlegmatic Trance** | `SPACE Key` | Active at 100 Composure. **5.0s invulnerability, zero stamina cost & auto-deflection aura**. |
| **3-Slot Weapon Hotbar** | `1, 2, 3 Keys` | Instantly swap between up to 3 weapons assigned to your hotbar slots mid-combat. |
| **Combat Quick Belt** | `H Key` / `4, 5, 6 Keys` | Consume Health Potions (`[H]`), Stoneskin Tonics (`[4]`), Glacial Thaw Draughts (`[5]`), and Berserker Brews (`[6]`). |
| **Interact / Advance** | `E Key` / `Enter` | Talk to NPCs, open chests, read monoliths, and transition doors. |
| **Storage Stash Trunk** | `T Key` | Open the persistent Storage Stash Vault at Zyphden Town Square or any realm campfire (or return from Arena). |
| **Tactical Radar Toggle** | `N Key` | Toggle the corner tactical mini-map radar and cardinal compass tape HUD. |
| **Dialogue Page** | `A Key` / `D Key` | Navigate between previous and next pages of active dialogue. |
| **Dismiss / Close** | `ESC Key` | Safely close active dialogue boxes, popups, and inventory modals. |
| **Objectives Tracker** | `O Key` | Toggle the pinned Chapter 1 quest objectives card. |
| **Inventory & Grimoire** | `TAB Key` (`I Key` alias) | Open Tactical Equipment Matrix, 3-Slot Weapon Hotbar assignment, armor loadouts, Lore Journal, Bestiary, and Realm Map. |
| **Humoral Bestiary** | `B Key` | Open the Inventory modal directly to the Humoral Bestiary & Mastery Perks tab. |
| **Interactive Realm Map** | `M Key` | Open tactical frosted teal map with live player tracking and stage POIs. |
| **Fullscreen Toggle** | `F11` or `Alt + Enter` | Switch between windowed and native crisp fullscreen display modes. |
| **Controls Guide** | `C Key` | Toggle the in-game keybindings overlay modal. |
| **Title Screen** | `~ Key` (Tilde / Grave) | Instantly return to the Title Screen from any state. |
| **Restart / Respawn** | `R Key` | Instantly respawn after defeat or restart the trial. |
| **Return from Arena** | `T Key` | Teleport from Combat Arena back to Knight Lenon in Zyphden. |

---

## Current Version: 0.0.1k

> [!NOTE]
> Phlegmatic is currently on **Version 0.0.1k**, succeeding Version 0.0.1j.
>
> ### Version 0.0.1k Highlights
> * **`[TAB]` Inventory, `[B]` Bestiary & Modal Fixes**: Restored `[TAB]` as Inventory (`[I]` alias preserved), fixed the `[M]` Realm Map instant-close regression, fixed Humoral Bestiary tab routing with a dedicated `[B]` shortcut, and enabled top-right HUD clicks in northern/eastern realms.
> * **3-Slot Weapon Hotbar & Rebound Quick Belt**: Equip up to 3 weapons via Inventory `[SLOT 1/2/3]` controls and switch mid-combat using `[1]`, `[2]`, `[3]` with full JSON save persistence. Potions moved cleanly to `[H]` (Heal), `[4]` (Stoneskin), `[5]` (Glacial Thaw), and `[6]` (Berserker Brew).
> * **Enlarged Vector-Icon HUD**: Upgraded bottom-center action slots (`58x62` / `64x68`) with procedural vector weapon/potion/armor skill icons and replaced the bottom-right box with an enlarged 3-Slot Weapon Hotbar Dock (`64x66` / `70x72` per slot).
> * **GLSL 330 Multiplicative Day-Night Shader**: Replaced flat color washes with an offscreen `RenderTexture2D` multiplicative lightmap, inverse-square point-light shaders, Purkinje composite tone-mapping, and full realm emitter registration.
> * **Combat & Balance Reworks**: Removed the broken stagger-on-every-hit execution loop (replacing Phlegmatic Core's stun with `3.0s` Chill); reworked **Twin Glacial Cleavers** (`4-stage Fury of Cleaves`), **Sun-Forged Claymore** (`2-stage Solar Cataclysm`), and **Storm Glaive** (`2-stage Tempest Cyclone`); revamped **Frostwarden** (`FROST NOVA`, `6.0s` `45 HP` `ICE WARD`, `30%` Cold Resist, `+20%` heavy poise); and rebalanced stamina/speed on Bow, Daggers, and **Totem of Mist**.
>
> For full technical breakdowns, system architectures, mathematical formulas, and complete version release notes, please consult **[DOCUMENTATION.md](documentation.md)** and **[VERSIONS.md](VERSIONS.md)**.

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
├── PROJECT_CONTEXT.txt         # Master project architecture & version context
├── CMakeLists.txt              # Cross-platform CMake configuration
├── assets/                     # Custom artwork & transition illustration drop
├── vendor/
│   └── raylib/                 # Precompiled Raylib 6.2 headers & static libraries
└── src/
    ├── main.cpp                # Game entry point & window lifecycle
    ├── core/                   # Engine config, procedural acoustic sound, particles
    ├── combat/                 # Weapons, armor pieces, sets, collision, projectiles, totems
    ├── world/                  # Level geometry, map environments, monoliths, chests
    ├── entities/               # Player, companion minion, enemy AI, modular vector renderers
    ├── rendering/              # Procedural vector silhouettes & asset loader
    ├── ui/                     # Dedicated HUD, modals, titles, and responsive UI components
    └── states/                 # State coordinators, handlers, and game logic
        └── realms/             # Modular realm exploration and encounter controllers
```

---

## License

This project is licensed under the [MIT License](LICENSE) - see the [`LICENSE`](LICENSE) file for details.  
Third-party library [Raylib](https://www.raylib.com/) is licensed under the zlib/libpng license.
