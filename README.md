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
| **Light Attack** | `Left Mouse Button` | Swift slash/thrust combo (Daggers 0 stam, Truth Seeker 2-hit combo, Elemental Totem piercing bolt). |
| **Heavy Signature Combo** | `Right Mouse Button` | Multi-stage weapon combos (Daggers 3-stage, Mace 2-stage, Crusader Blade 3-stage, Truth Seeker 2-stage, Halberd 3-stage). |
| **Armor Set Special Skill** | `V Key` | Triggers active skill on 3/3 matching armor sets (Dust Burst, Dawnfire Aegis, Seismic Quake, Shadow Decoy, Spike Wall, Basalt Spires). |
| **Dash (Evade)** | `Q Key` | Fast directional burst with **0.25s i-frames** (`EVADED`). |
| **Timed Parry** | `F Key` | Active guard window. Staggers melee foes and **reflects incoming projectiles**. |
| **Phlegmatic Trance** | `SPACE Key` | Active at 100 Composure. **5.0s invulnerability, zero stamina cost & auto-deflection aura**. |
| **Health Potion** | `1 Key` or `H Key` | Consumes 1 elixir to heal **+45 HP** immediately (**7.0s cooldown**). |
| **Interact / Advance** | `E Key` / `Enter` | Talk to NPCs, open chests, read monoliths, and transition doors. |
| **Dialogue Page** | `A Key` / `D Key` | Navigate between previous and next pages of active dialogue. |
| **Dismiss / Close** | `ESC Key` | Safely close active dialogue boxes, popups, and inventory modals. |
| **Objectives Tracker** | `O Key` | Toggle the pinned Chapter 1 quest objectives card. |
| **Equipment & Grimoire** | `I Key` or `TAB` | Open Tactical Equipment doll, single Totem slot, Weapons arsenal, and Illuminated Book Lore Journal. |
| **Interactive Realm Map** | `M Key` | Open tactical frosted teal map with live player tracking and stage POIs. |
| **Fullscreen Toggle** | `F11` or `Alt + Enter` | Switch between windowed and native crisp fullscreen display modes. |
| **Controls Guide** | `C Key` | Toggle the in-game keybindings overlay modal. |
| **Title Screen** | `~ Key` (Tilde / Grave) | Instantly return to the Title Screen from any state. |
| **Restart / Respawn** | `R Key` | Instantly respawn after defeat or restart the trial. |

---

## Current Version: 0.0.1e

> [!NOTE]
> Phlegmatic is currently on **Version 0.0.1e**.
>
> ### Version 0.0.1e Release Highlights
> * **Modular 3-Piece Armor System & Full Set Bonuses**:
>   - Replaced monolithic suits with 3 modular equippable slots: **Helmet**, **Chestplate**, and **Leggings** (strictly NO boots, gloves, or rings).
>   - Equipping a matching 3/3 set activates unique passive stat bonuses and an active signature skill triggered by the **`[V]`** key:
>     - **Commoner's Scrap**: *Scrapper's Tenacity* (+15% Stamina Regen, +15% Shingle/Ore drops) | Skill: *Desperate Dust Burst* (30s CD, 180px 1.5s stun AoE, +35% move speed for 4s).
>     - **Crusader Plate**: *Aegis of the Sun* (+20% Defense, +25 Will) | Skill: *Dawnfire Aegis* (35s CD, 60 HP radiant barrier absorbing all strikes with holy detonation).
>     - **Vanguard Bastion**: *Unstoppable Juggernaut* (+40 Max HP, hyper-armor micro-stagger immunity during heavy attacks) | Skill: *Seismic Quake Slam* (22s CD, 200px 50 dmg shockwave, -25% armor shred, knocks foes flat).
>     - **Shadowstrider**: *Veil of the Mist* (+15% Move Speed, -35% dash CD, +25% dash distance) | Skill: *Shadow Decoy & Vanish* (25s CD, 0.5s i-frames, leaves illusion decoy drawing enemy aggro, explodes for 40 shadow dmg).
>     - **Forged Scrap Iron**: *Tempered Slag* (+18 Defense, +15% poise damage) | Skill: *Ironclad Spike Wall* (25s CD, 6s defensive stance reflecting 40% melee damage with hyper-armor).
>     - **Basalt Golem**: *Primordial Earth Carapace* (+35 Defense, -10% Speed, Melancholic Shell absorbing 1 heavy hit every 20s) | Skill: *Basalt Spire Wall* (30s CD, summons 3 impenetrable basalt pillars blocking enemy movement and projectiles, dealing 55 earth dmg).
> * **Tactical Equipment HUD Redesign (Image Reference)**:
>   - Redesigned Tab 0 featuring `EQUIPPED` header, `APPAREL HIDDEN` cosmetic toggle, central character silhouette doll flanked by Helmet (top), Chestplate (mid-left), and Leggings (mid-right).
>   - Single equippable `TOTEMS` slot (only 1 totem can be active at a time).
>   - `WEAPONS` quick-slot row with active set bonus banner and slide-out Selection Drawer with one-click equip for unlocked armaments and armor.
>   - **Illuminated Book Lore Journal**: Fully preserved in Tab 1 with 3D parchment turn animations, golden drop caps, and runic seals.
> * **Blacksmith Varek Armor Forge Expansion**:
>   - Added Tab 3 `[3] ARMOR FORGE (6)` to Varek's forge in Zyphden. Craft Forged Scrap Iron and Basalt Golem armor pieces using collected Ore Scraps and Shingles.
> * **Quarry Arena & Elemental Golem Boss Overhaul**:
>   - Relocated Ore Golem spawners south away from the altar arena to prevent crowding.
>   - Elemental Golem is now repeatable/respawnable at the Primordial Altar whenever not currently active on the field.
>   - Altar boss totem drop rate set to **25%** for `ELEMENTAL_CORE_TOTEM`.
>   - Revamped boss attack summoning up to 3 `MINI_ELEMENTAL_GOLEM`s on a 22s cooldown.
> * **Revamped Elemental Core Totem & Friendly Companion Minion**:
>   - Automatically spawns a friendly Mini Elemental Golem companion with **75% stats** of the boss's minion (105 HP, 14 DMG, 11 DEF).
>   - Companion follows the player, attacks hostile enemies, and absorbs damage.
>   - If shattered, companion enters a **60-second respawn timer**, tracked in real time via a dedicated HUD Companion Status Widget above the player card.
>   - Light attack projectile upgraded to **infinitely piercing**, damage increased to **54.0**, and cooldown reduced to **3.0s**.
> * **Weapon Heavy Attack Combo Revamp & Damage Rebalancing**:
>   - **Dual Daggers** (12.0 light / 58.0 heavy): 3-stage combo — *Shadow Puncture* (18 dmg, 0.2s iframes, applies 2.5s Bleed) -> *Backstab Cross-Slash* (28 dmg, +40% bonus against bleeding/rear targets) -> *Vault Kick* (12 dmg, 100% poise stagger, backward leap impulse).
>   - **War Mace** (24.0 light / 78.0 heavy): 2-stage combo — *Overhead Skull Crusher* (42 dmg, hyper-armor, inflicts -20% Armor Fracture debuff for 4s) -> *Seismic Tremor* (36 dmg, 360° circular shockwave, 25% defense pierce, knocks foes flat).
>   - **Crusader Blade** (18.0 light / 88.0 heavy): 3-stage combo — *Crescent Cleave* (30 dmg, restores +12 Composure) -> *Shock Thrust* (24 dmg, piercing line interrupting enemy windups) -> *Radiant Execution* (34 dmg, downward holy cleave dealing +50% bonus damage to staggered foes).
>   - **Truth Seeker** (22.0 light / 72.0 heavy): 2-stage combo — *Dawnfire Ring* (34 dmg, 360° sweep applying Solar Burn & firing holy beam) -> *Sacred Judgment Pillar* (38 dmg, descending sunray beam with 100% defense pierce that cleanses player debuffs).
>   - **Royal Halberd** (16.0 light / 28.0 heavy): 3-stage combo — *Long Lunge* with iframes -> *Sweeping Cleave* (220° arc) -> *Flurry* of 3 consecutive rapid stabs (30% weapon damage each).
> 
> For detailed release notes, system mechanics breakdowns, math formulas, and lore codices, please consult **[DOCUMENTATION.md](documentation.md)**.

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
    └── states/                 # State coordinators, handlers, and modular HUD/UI
```

---

## License

This project is licensed under the [MIT License](LICENSE) - see the [`LICENSE`](LICENSE) file for details.  
Third-party library [Raylib](https://www.raylib.com/) is licensed under the zlib/libpng license.
