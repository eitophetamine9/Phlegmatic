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
| **Phlegmatic Trance** | `SPACE Key` | Active at 100 Composure. **5.0s invulnerability, zero stamina cost & auto-deflection aura**. |
| **Health Potion** | `1 Key` or `H Key` | Consumes 1 elixir to heal **+45 HP** immediately (**7.0s cooldown**). |
| **Interact / Advance** | `E Key` / `Enter` | Talk to NPCs, open chests, read monoliths, and transition doors. |
| **Dialogue Page** | `A Key` / `D Key` | Navigate between previous and next pages of active dialogue. |
| **Dismiss / Close** | `ESC Key` | Safely close active dialogue boxes, popups, and inventory modals. |
| **Objectives Tracker** | `O Key` | Toggle the pinned Chapter 1 quest objectives card. |
| **Lore & Equipment** | `I Key` or `TAB` | Open the in-game Journal, Equipment stats, and Totem slots. |
| **Controls Guide** | `C Key` | Toggle the in-game keybindings overlay modal. |
| **Title Screen** | `~ Key` (Tilde / Grave) | Instantly return to the Title Screen from any state. |
| **Restart / Respawn** | `R Key` | Instantly respawn after defeat or restart the trial. |

---

## What's New in Version 0.0.1b

### 1. Medieval Typography & UTF-8 Character Sanitizer
* **Custom Typography Engine (`FontManager`)**: Replaced default fonts with an atmospheric medieval typeface tailored for dark fantasy roleplay.
* **Glyph Fallback & Sanitizer**: Embedded unicode sanitizer ensures special punctuation, dashes, quotes, and symbols render cleanly without question mark (`?`) artifacts.

### 2. Multi-Slot Save Architecture
* **3-Slot Save Profiles**: Manage three independent campaign saves (`save_slot_1.dat`, `save_slot_2.dat`, `save_slot_3.dat`).
* **Title Record Modal**: Visual slot selector on the Title Screen displaying active chapter location, equipped armaments, and play metrics with safe deletion confirmation.
* **Automatic State Persistence**: Real-time autosaving across transitions, boss milestones, and vendor purchases.

### 3. Silver Shingles Economy & Physical Loot Drops
* **Currency System**: Earn **Silver Shingles** by defeating corrupted foes and exploring the realms of Tydeshire.
* **Physical Coin Drops**: Slain enemies drop physical shining silver coins with magnetic vacuum attraction toward the player.
* **Roaming Enemy Respawns**: Overworld hostiles respawn on periodic cycles, allowing deliberate resource farming.

### 4. Blacksmith Varek's Forge & Weapon Upgrades
* **Interactive Forge Modal**: Visit Blacksmith Varek in Zyphden to temper and sharpen your arsenal.
* **Weapon Upgrade Tiers**:
  * **Standard (Base)**: Baseline weapon attributes.
  * **Tier 1 (+1 Refined)**: +20% base damage, enhanced poise multipliers.
  * **Tier 2 (+2 Masterwork)**: Critical damage bonuses and extended attack reach.
  * **Consecrated Relic**: High armor penetration and sacred projectile waves.

### 5. Tavernkeep Eldon's Provisions
* **Hearthside Restock Counter**: Speak with Tavernkeep Eldon in Zyphden to purchase replenishing **Health Elixirs** using Silver Shingles.

### 6. Phlegmatic Composure & Four Humors Alignment
* **Composure Gauge (0-100)**:
  * Master deliberate combat: gains +25 on timed parries, +15 on dash evasions, +30 on quenching Choleric frenzy, and +8/s in Patient Stance.
  * Depleted by unmitigated heavy hits (-25) and attacking when exhausted (-15).
* **Poise Tiers**:
  * **Discomposed (<30)**: Red aura, +15% stagger duration vulnerability.
  * **Composed (30-80)**: Balanced combat equilibrium.
  * **Stoic Poise (80-100)**: Golden radiance granting complete **Hyper-Armor** (immunity to stagger and bleed during heavy attack windups).
* **Active Ability — Phlegmatic Trance (`[SPACE]`)**:
  * Unleashed at 100 Composure. Grants 5.0 seconds of complete invulnerability, zero stamina consumption, and an automatic 120px projectile reflection sphere.
* **Medieval Four Humors Alignment**:
  * **Choleric (Fire)**: High aggression; parrying quenches their fury (-30% speed, 2.0x stagger).
  * **Melancholic (Earth/Void)**: Heavy defense shells; shattered by War Mace or heavy cleaves (-35% armor).
  * **Sanguine (Blood)**: Inflicts arterial bleed; resisted by Stoic Poise.
  * **Phlegmatic (Water)**: Calculated composure and steadfast discipline.

### 7. Combat Juice & Feedback
* **Hit-Stop Micro-Pause**: 0.04s - 0.06s freeze-frame impacts on parry deflections, heavy cleaves, and armor breaks.
* **Directional Camera Shake**: Smooth directional impulse and recoil physics reacting to the angle of each heavy strike and deflection.

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
