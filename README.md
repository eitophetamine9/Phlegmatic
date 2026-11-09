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
| **Aiming** | `Mouse Cursor` | Reticle targeting for weapon thrusts, dashes, and parry reflections. |
| **Light Attack** | `Left Mouse Button` | Swift slash/thrust combo (Daggers every 5th strike consumes 14 stamina, Truth Seeker 2-hit combo, Elemental Totem piercing bolt). |
| **Heavy Signature Combo** | `Right Mouse Button` | Multi-stage weapon combos (Daggers 3-stage, Mace 2-stage, Crusader Blade 3-stage, Truth Seeker 2-stage, Halberd 3-stage). |
| **Armor Set Special Skill** | `V Key` | Triggers active skill on 3/3 matching armor sets (Dust Burst, Dawnfire Aegis, Seismic Quake, Shadow Decoy, Spike Wall, Basalt Spires, Glacial Frost Nova). |
| **Dash (Evade)** | `Q Key` | Fast directional burst with **0.25s i-frames** (`EVADED`). |
| **Timed Parry / Execution** | `F Key` | Active guard window. Staggers melee foes, reflects projectiles, and performs **Visceral Execution Deathblows** on posture-broken targets. |
| **Phlegmatic Trance** | `SPACE Key` | Active at 100 Composure. **5.0s invulnerability, zero stamina cost & auto-deflection aura**. |
| **Combat Quick Belt** | `1, 2, 3 Keys` | Rapid consumption of Health Potions, Stoneskin Tonics, Glacial Draughts, or Berserker Draughts. |
| **Interact / Advance** | `E Key` / `Enter` | Talk to NPCs, open chests, read monoliths, and transition doors. |
| **Storage Stash Trunk** | `T Key` | Open the persistent Storage Stash Vault at Zyphden Town Square or any realm campfire. |
| **Tactical Radar Toggle** | `N Key` | Toggle the corner tactical mini-map radar and cardinal compass tape HUD. |
| **Dialogue Page** | `A Key` / `D Key` | Navigate between previous and next pages of active dialogue. |
| **Dismiss / Close** | `ESC Key` | Safely close active dialogue boxes, popups, and inventory modals. |
| **Objectives Tracker** | `O Key` | Toggle the pinned Chapter 1 quest objectives card. |
| **Quick Weapon Swap** | `TAB Key` | Instantly cycle through all unlocked weapons in your arsenal mid-combat. |
| **Equipment & Grimoire** | `I Key` | Open Tactical Equipment doll, modular armor loadouts, weapons arsenal, and Illuminated Book Lore Journal. |
| **Interactive Realm Map** | `M Key` | Open tactical frosted teal map with live player tracking and stage POIs. |
| **Fullscreen Toggle** | `F11` or `Alt + Enter` | Switch between windowed and native crisp fullscreen display modes. |
| **Controls Guide** | `C Key` | Toggle the in-game keybindings overlay modal. |
| **Title Screen** | `~ Key` (Tilde / Grave) | Instantly return to the Title Screen from any state. |
| **Restart / Respawn** | `R Key` | Instantly respawn after defeat or restart the trial. |
| **Return from Arena** | `T Key` | Teleport from Combat Arena back to Knight Lenon in Zyphden. |

---

## Current Version: 0.0.1j (In-Development)

> [!NOTE]
> Phlegmatic is currently on **Version 0.0.1j (In-Development)**, succeeding Version 0.0.1i.
>
> ### Version 0.0.1i Milestone Highlights
> * **Northern Arsenal & Frostwarden Armor**: Glacial Cleaver colossal great-axe with heavy Frost Tremor shockwaves and the 3-piece Frostwarden set with Glacial Tenacity passive and Glacial Frost Nova active skill.
> * **Highland Weather & Procedural Audio**: Procedural howling blizzard ambient audio synthesis, snow crunch footsteps, blizzard particle stream, and breakable frost crystal deposits.
> * **Frostbite Environmental Chill System**: Sub-zero temperature accumulation inflicting Frostbite (-25% speed, -40% stamina regen) with bonfire thawing and Frostwarden cold immunity.
> * **Outcast Ranger Torvald NPC & Bounties**: Stationed near the Highland Bonfire offering 3 tiered hunting contracts with real-time progression and rewards.
> * **Quick Weapon Swap & Apex Boss Gauntlet**: Instant weapon cycling mid-combat on `[TAB]` and Knight Lenon's 3-stage Apex Boss Gauntlet rush mode.
>
> ### Version 0.0.1j Implementation Roadmap
> 1. **Phase 1**: Visceral Execution & Stagger Deathblow System [COMPLETED]
> 2. **Phase 2**: Alchemy Workshop & Quick-Item Belt (Tavern Keeper Brena) [COMPLETED]
> 3. **Phase 3**: The 7th Sacred Relic: Totem of the Phlegmatic Core [COMPLETED]
> 4. **Phase 4**: Dynamic Time-of-Day Lighting Cycle & Nightfall Incursions [COMPLETED]
> 5. **Phase 5**: Tactical Corner Mini-Map Radar & Compass HUD [COMPLETED]
> 6. **Phase 6**: Bestiary Mastery Hunting Perks & Codex Seals [COMPLETED]
> 7. **Phase 7**: Subterranean Highland Caverns Sub-Zone (The Permafrost Crypts) [COMPLETED]
> 8. **Phase 8**: Storage Stash Chest & Persistence Verification [COMPLETED]
> 9. **Phase 9**: File Architecture Restructuring & Subsystem Decoupling [COMPLETED]
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
