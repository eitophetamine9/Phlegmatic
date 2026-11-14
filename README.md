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

> *"For over 500 years, the Land of Tydeshire was consumed by unending war until King Charlz unified the realm, crowning himself the Liege of Kings. He divided the continent into five sovereign realms to preserve peace: Zyphden, Azshden, Fleurshden, Noirden, and Solsden.*
>
> *Yet peace in Tydeshire was only ever borrowed. Three decades later, the Lord Liege vanished into madness, plunging the realm into paranoia and blood. As the ancient warding seals fracture, the true history of his fall lies scattered across forgotten steles and sunlit ruins, waiting for one who can pierce the illusions."*

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
| **3-Slot Weapon Hotbar** | `1, 2, 3 Keys` or `Click` | Instantly swap between up to 3 weapons assigned to your hotbar slots mid-combat (via hotkeys or clicking HUD dock). |
| **Equipment Loadout Presets** | `[ I ]`, `[ II ]`, `[ III ]` | Snapshot and switch complete loadouts (weapon, armor, totem) in Equipment Matrix with persistent reboot saves. |
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

## Current Version: 0.0.1l

> [!NOTE]
> Phlegmatic is currently on **Version 0.0.1l**, succeeding Version 0.0.1k.
>
> ### Version 0.0.1l Highlights
> * **Apex Boss Gauntlet Difficulty Escalation**: Pinnacle challenge tuning for Remnant Zil (2,200 HP, 50 DEF), Primordial Titan (3,400 HP, 75 DEF), and Berserker Troll Warlord (4,800 HP, 58 DEF) with 1.85x-2.0x damage multipliers, 25% faster windups, 30% lower recovery, deadlier wave reinforcements, and an intense <35% HP Enrage Phase (+20% speed, -15% windup, -25% recovery, crimson fury aura).
> * **3-Tier Totem Leveling & Commanding Titan Evolution**: All 8 primal totems can now be leveled across Tier I, Tier II, and Tier III at Blacksmith Varek's Forge using Shingles and Ore Scraps. At Tier 3, the Elemental Core Totem evolves the mini golem companion into the colossal **Commanding Elemental Golem** titan (920 HP, 75 DEF), which commands and summons 3 orbiting Mini Golems (280 HP, 40 DEF, 18 DMG) with independent combat AI and auto-resurrection.
> * **Sovereign's Scepter (Weapon 12) & Damage Scaling Rebalance**: Pure restorative weapon unlocked at Arena Wave 25 dealing negligible offensive damage (4 DMG light, 0 DMG heavy) while delivering vital support to summons (+24 HP targeted beam; +75 HP full 360-degree AoE Sanctuary Radiance radius heal with debuff cleansing and defensive/speed buffs). Standardized weapon damage scaling across +0/+1/+2 tiers for all 11 offensive weapons.
> * **Commander's Totem, Loose Spacing & Exact Enemy Silhouettes**: Conquering standard Wave 25 awards the **Commander's Totem** (8th attunable primal relic, +20% DEF, +40 Max HP, +25 Will, -40% player direct damage penalty), commanding up to 7 permanent tower gladiators. Companions feature loosened escort spacing (135px orbit, 90px personal space repulsion from player) and are rendered using 1:1 exact procedural silhouettes and gear matching true in-game hostiles.
> * **Procedural Shadowstrider Decoy**: Overhauled decoy clone into a distinct procedural hooded silhouette with glowing violet slit eyes, crossed spectral daggers, and lifetime countdown ring.
> * **Modal Inspection Card Auto-Wrap**: Overhauled modal relic drawer cards with dynamic vertical flow and dual text wrapping, completely eliminating horizontal line overflows and vertical collisions with lore text.
> * **World Progression Gating & Real-Time Background Cooldowns**: Western Mist Forest and Northern Crags are physically locked until Captain Lenon is defeated in courtyard sparring. Boss rematch timers, companion squad resurrection timers, player skill and potion cooldowns, day/night cycles, and arena spawners continue ticking in real-time even while inside inventory, bestiary, or map modals.
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
