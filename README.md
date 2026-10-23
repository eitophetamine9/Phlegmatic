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

</div>

---

## The World & Lore: Land of Tydeshire

> *“For over 500 years, the Land of Tydeshire was filled with chaos and never ending war. Order was not an option, there was no unity. Factions came along and so then became kingdoms. Each king wants to rule over the entire Land of Tydeshire, most failed miserably however one stood at the top. After 500 years of chaos, one king stood at the top, King Charlz. His overwhelming authority and remarkable strength unified the Land of Tydeshire crowning him Charlz the Lord Liege.*
>
> *He then proceeded to divide the land into 5 Kingdoms (Azshden, Fleurshden, Zyphden, and Noirden) each having kings to rule over each. However after 3 decades of peace, the Lord Liege himself seemed to have gone mad and is now dictating the entire Land of Tydeshire. Formerly known as the Land of Peace became the Land of Chaos. The true history of the entire ordeal has been scattered into fragments, waiting to be uncovered for meaning.”*

---

## Key Features

* **Prologue Cinematic Cutscene**:
  * An atmospheric dark fantasy opening narrative with slow-drifting embers, cathedral abbey chime acoustics, and cinematic text crawl (`[SPACE]` / `[ENTER]` to advance, `[ESC]` to skip).
* **Chapter 1: The Ruined Castle of Zyphden**:
  * Atmospheric overgrown castle sanctuary complete with mossy stone flagstones, carved colonnades, guardian statues, and an elevated high altar.
  * **Interactive Tutorial Pop-up Modals**: High-priority illuminated parchment pop-ups that temporarily pause real-time combat and gameplay, giving players ample time to read movement, swordplay, and defense instructions. Dismissable instantly via `[SPACE]`, `[ENTER]`, or `RESUME` button.
* **Lore Journal & Chronicles Modal (`[I]` / `[TAB]`)**:
  * Built-in interactive codex and royal inventory.
  * Reads discovered historical fragments recovered from ancient relic chests and fallen champions.
  * Gold notification badges signal newly uncovered chronicles.
* **Boss Encounter: Zil the Corrupted Crusader**:
  * Duel the guardian of the north portcullis atop the high dais.
  * Features parriable strikes, shield bashes, and flame wave projectiles.
  * Branching defeat mechanics: 1-time duel retry or sparing by Zil to continue your journey into the open wilderness.
  * Defeating Zil rewards the legendary broadsword **Truth Seeker** and Fragment III.
* **Precision Combat System**:
  * **`Q` Key — Dash**: 8-directional burst with **invulnerability frames (i-frames)** to phase safely through attacks and projectiles (`EVADED`).
  * **`F` Key — Timed Parry**: Active 0.28s guard window with radiant cyan-gold shield barrier. Deflecting melee blows staggers enemies for 2.0s (`DEFENSE BROKEN!`) and grants a **+50% Counter-Attack Damage Buff**.
  * **Projectile Deflection**: Parrying incoming enemy spells and arrows reflects them back toward your cursor with amplified speed and damage.
* **Procedural Medieval Acoustic Audio**:
  * 100% custom-synthesized medieval soundscape with zero retro/sci-fi synth beeps:
    - Cathedral abbey bells with acoustic overtone partials.
    - Tempered steel blade cutting swooshes.
    - Anvil & broadsword clashes with metallic ringing.
    - Chainmail & plate armor blunt impact thuds.
    - Crossbow string releases with wooden timber creaks.
    - Deep medieval war horns and portcullis iron chain rumbles.
* **Combat Arena & Wave Defense Mode (Expanded to 5 Waves)**:
  * 5 escalating combat waves featuring mixed vanguard formations, high-threat charger vanguards, dual-brute shockwaves, kiting snipers, and culminating in **The Grand Inquisitor** boss trial on Wave 5.
  * **Adhesive Bandages (+30 HP)**: Glowing medical pickups spawn periodically across the arena floor and drop upon defeating enemies, providing tactical healing sustain during prolonged trials.
* **Test Arena Mode (Main Menu)**:
  * Unlocked on the Title Screen once the player either **defeats Zil** or **faces him in duel and accepts his spare**.
  * Grants instant access to all 5 arena combat trials directly from the main menu for continuous weapon and defense testing.
* **Streamlined Objectives HUD**:
  * Tutorial quest objectives are cleanly pinned to the top-left margin to preserve central combat visibility.
* **Armor Defense & Mitigation System**:
  * Both player and enemy archetypes possess innate armor defense values that attenuate incoming damage using diminishing returns:
    $$\text{Damage} = \text{RawDamage} \times \left(\frac{100}{100 + \text{Defense} \times (1 - \text{Pierce})}\right)$$
  * The player character starts with **15 Defense (~13% mitigation)**, visible in the Lore & Inventory codex (`[I]`). Certain heavy strikes (e.g., War Mace Earthshaker, Truth Seeker Holy Beam) feature defense penetration.

---

## Controls

| Action | Key / Input | Gameplay Role |
| :--- | :--- | :--- |
| **Movement** | `W, A, S, D` or `Arrow Keys` | 8-directional fluid traversal with solid wall and pillar collisions. |
| **Aiming** | `Mouse Cursor` | Reticle targeting for sword thrusts, dashes, and parry reflections. |
| **Light Attack** | `Left Mouse Button` | Swift slash/thrust. Low/zero stamina cost, rapid recovery cadence. |
| **Heavy Cleave** | `Right Mouse Button` | High-damage signature moves (multi-stage attacks, lunges, shockwaves). |
| **Dash (Evade)** | `Q Key` | Fast directional burst with **i-frames** (`EVADED`) to dodge sweeps & missiles. |
| **Timed Parry** | `F Key` | 0.28s guard. Staggers melee foes for 2.0s and **reflects incoming projectiles**. |
| **Interact** | `E Key` / `Left Click` | Claim weapons, open ancient relic chests, and trigger portal transit. |
| **Lore Journal** | `I Key` or `TAB` | Toggle the in-game chronicles codex, armor stats, and weapon inventory. |
| **Controls Guide** | `C Key` | Toggle the quick-reference controls overlay modal. |
| **Restart / Respawn** | `R Key` | Instantly respawn after defeat or restart the combat trial. |
| **Return to Lobby** | `L Key` or `ESC` | Return to the Armory Sanctuary. |

---

## Weapon Arsenal

| Weapon | Stance Style | Light Atk | Heavy Atk | Dash Cooldown | Passives & Special Properties |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Unarmed** | Bare Fists | None | None | 0.75s (15 stam) | Starting tutorial state. Search the courtyard altar to claim a weapon. |
| **Crusader's Broadsword** | Balanced Steel | 25 dmg (110° arc, 14 stam) | 85 dmg (2-Stage cleave) | 0.85s (20 stam) | Dependable reach. **Heavy**: Stage 1 wide 150° cleave followed by Stage 2 upward shock slash line/box AoE (2.1s cd). |
| **Truth Seeker** | Legendary Holy Blade | 46 / 60 dmg (2-Hit Combo) | 140 dmg (Dual 360° + Holy Beam) | 0.65s (16 stam) | **Light**: Slash into radiant upward slice (line/box AoE). **Heavy**: Rapid dual 360° dawnfire sweeps + holy beam projectile (10% defense pierce, 3.8s cd). |
| **Dual Daggers** | Agility Flurry | 22 dmg (twin slice, **0 stam**) | 55 dmg (Lunge + Instant Spin) | 0.50s (14 stam) | Zero stamina light attack cadence. **Heavy**: High-velocity forward lunge burst into instant 360° whirlwind (1.8s cd). |
| **War Mace** | Crushing Poise | 52 dmg (Frontal Circle Smack) | 155 dmg (Earthshaker 2-Stage) | 1.25s (26 stam) | High damage bludgeon. **Light**: Frontal circular smack. **Heavy**: Amplified frontal smack followed by 360° tremor shockwave (**ignores 25% enemy defense**, 3.6s cd). |

> [!NOTE]
> In **Chapter 1: Ruined Castle**, the player begins unarmed with zero weapons in inventory. The Crusader's Broadsword is claimed at the central altar plinth, and the legendary **Truth Seeker** is claimed by triumphing over Zil the Corrupted Crusader. After the tutorial, Truth Seeker is permanently retained in your arsenal for the Hub and Test Arena.

---

## Enemies & Boss Encounters

| Enemy | Archetype | Defense | Telegraph Shape | Combat Behavior & Defeat Strategy |
| :--- | :--- | :--- | :--- | :--- |
| **Cultist Slasher** | Melee Vanguard | 8 | Amber/Red Cone (95°, 115px) | Forward blade slash. Sidestep or time `[F]` right before impact to stagger. |
| **Deathseeker Archer** | Ranged Kiting | 6 | Red Laser Aim Line (360px range) | Backs away if approached; fires high-velocity piercing bolts. |
| **Void Pyromancer** | Occult Caster | 10 | 3-Way Trajectory Aim Lines (320px) | Shows exact projectile flight paths. Fires 3 void orbs. Evade with `[Q]` or reflect with `[F]`. |
| **Shadow Charger** | Dash Melee | 12 | Linear Path (50px width, 240px) | High-speed dash thrust. Step perpendicular to path or parry head-on. |
| **The Grand Inquisitor** | **Arena Boss** | 26 | Multi-Phase Trajectories & Tremor AoE | 500 HP boss rotating between **12-way Radial Star Trajectories**, **Linear Beam Shard**, and **Cataclysmic Smite AoE**. |
| **Zil the Corrupted Crusader** | **Chapter 1 Boss** | 28 | Cone Cleave, Charge Path & 3-Way Flame Trajectories | 380 HP fallen crusader guarding the portcullis. Dynamic moveset with melee cleaves, shield charges, and 3-way flame waves. |
| **Iron Brute** | Heavy Crusher | 32 | Expanding Circle (150px AoE) | High armor tank. Tremor ground slam. Dash clear with `[Q]` or pierce armor with War Mace Earthshaker. |

> [!TIP]
> **Visual Clarity Rule**: Filled Cone AoEs are reserved strictly for attacks that deal direct melee/cleave damage within that sector. Projectiles display precise trajectory laser lines indicating their exact flight paths, allowing clean evasion between projectiles.

---

## Quick Start & Running

Standalone precompiled 64-bit Windows executables are included in the repository:

### Running the Game:
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
* **Automated Batch Build (MinGW-w64 GCC)**:
  ```powershell
  .\build.bat
  ```
  *Compiles all C++ source files with `-O2 -static` and outputs both `phlegmatic.exe` and `bin\phlegmatic.exe`.*

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
    │   ├── Config.hpp          # Game constants, window dimensions, weapon baseline stats
    │   ├── SoundFX.hpp / .cpp  # Medieval acoustic audio synthesis (steel clangs, bells, horns)
    │   └── Particles.hpp / .cpp# Screen shake, floating combat text, spark bursts
    │
    ├── combat/                 # Weapons, combat math, inventory, and projectile engine
    │   ├── CombatTypes.hpp     # AttackInfo, TelegraphIndicator, enums
    │   ├── Weapon.hpp          # Broadsword, Daggers, Mace, and Truth Seeker definitions
    │   ├── Inventory.hpp / .cpp# Lore fragment collection, journal tracking, unread flags
    │   ├── CollisionSystem.hpp / .cpp# Melee hit registration, i-frame evade, parry reflection
    │   └── Projectile.hpp / .cpp# Real-time projectile physics, walls, parry reflection
    │
    ├── world/                  # Level geometry and interactive world objects
    │   ├── Map.hpp / .cpp      # Ruined Castle, Lobby sanctuary, and Arena layouts
    │   ├── WaveManager.hpp / .cpp# Wave progression, enemy spawning, and arena trials
    │   └── Interactable.hpp    # Weapon pedestals, training dummy, arena portal
    │
    ├── entities/               # Player character and enemy AI state machines
    │   ├── Player.hpp / .cpp   # WASD movement, Q Dash (i-frames), F Parry, combat state
    │   ├── PlayerRenderer.hpp / .cpp# Player visual renderer (weapon sweeps, whirlwind, silhouettes)
    │   ├── Enemy.hpp / .cpp    # 5 enemy archetypes + Zil Boss & The Grand Inquisitor AI
    │   └── EnemyRenderer.hpp / .cpp# Enemy telegraph geometric visuals & boss aura rendering
    │
    ├── rendering/              # Procedural silhouettes and asset loader
    │   └── AssetManager.hpp / .cpp# Sprite loader + procedural vector silhouettes
    │
    └── states/                 # State management and UI rendering
        ├── GameState.hpp       # State enums (TITLE_SCREEN, PROLOGUE_CUTSCENE, RUINED_CASTLE, etc.)
        ├── Game.hpp / .cpp     # Master game loop & state coordinator
        ├── RuinedCastleHandler.hpp / .cpp# Chapter 1 Ruined Castle loop & Zil Crusader duel branching
        ├── Cutscene.hpp / .cpp # Prologue dark fantasy narrative renderer
        ├── Tutorial.hpp / .cpp # Chapter 1 step tracker & interactive pop-up modals
        ├── TitleUI.hpp / .cpp  # Title screen with Campaign & Test Arena buttons
        ├── HUD.hpp / .cpp      # Combat HUD, Lobby HUD, Boss health & dialogue banners
        ├── ModalUI.hpp / .cpp  # Controls modal, Lore Journal, Zil defeat & gateway epilogue
        └── UI.hpp / .cpp       # Unified UI facade maintaining backwards-compatible API
```

---

## License

This project is licensed under the [MIT License](LICENSE) - see the [`LICENSE`](LICENSE) file for details.  
Third-party library [Raylib](https://www.raylib.com/) is licensed under the zlib/libpng license.
