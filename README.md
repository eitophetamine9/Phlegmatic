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

> *“For over 500 years, the Land of Tydeshire was filled with chaos and never ending war. Order was not an option, there was no unity. Factions came along and so then became kingdoms. Each king wants to rule over the entire Land of Tydeshire, most failed miserably however one stood at the top. After 500 years of chaos, one king stood at the top, King Charlz. His overwhelming authority and remarkable strength unified the Land of Tydeshire crowning him Charlz the Liege of Kings.*
>
> *He then proceeded to divide the land into 5 Kingdoms (Azshden, Fleurshden, Zyphden, and Noirden) each having kings to rule over each. However after 3 decades of peace, the Lord Liege himself seemed to have gone mad and is now dictating the entire Land of Tydeshire. Formerly known as the Land of Peace became the Land of Chaos. The true history of the entire ordeal has been scattered into fragments, waiting to be uncovered for meaning.”*

---

## Key Features

* **Prologue Cinematic Cutscene**:
  * An atmospheric dark fantasy opening narrative with slow-drifting embers, cathedral abbey chime acoustics, and cinematic text crawl (`[SPACE]` / `[ENTER]` to advance, `[ESC]` to skip).
* **Multi-Stage Campaign Progression (Chapter 1)**:
  * **Act I: The Ruined Castle**: Overgrown sanctuary, tutorial pop-up modals, sanctuary ward barrier, and the high-altar trial against **Zil's Fragment** awarding the legendary **Truth Seeker (Partial)**.
  * **Act II: The Crossroads of the Damned**: Void-corrupted wilderness connecting the southern ruins to the northern realm. Features telepathic monster ambushes with directional telegraphs and the ancient shrine holding the **Totem of Sanity**.
  * **Act III: Fortified Realm of Zyphden**: Walled capital of the Gale Tempests featuring cobblestone highways, town noticeboard, lore historians (Chronicler Cedric, Blacksmith Varek, Elder Maeve), and collectible royal archives.
  * **Hearthside Tavern**: Cozy interior retreat with stone hearth fireplace, timber bar counter, stools, and Tavernkeep Eldon, featuring a scripted entrance sequence for Knight Lenon.
  * **Zyphden Royal Keep (Throne Hall)**: Grand interior throne room with polished marble flagstones, crimson runner carpet, royal braziers, and **King Kreptu** presiding from his golden throne dais to grant royal sanction.
  * **Courtyard Boss Spar — Knight Lenon**: Martial duel in the Eastern Courtyard against the Vanguard Captain, awarding the consecrated **Crusader Plate (+24 Def, +30 HP)** and unlocking the Royal Combat Arena.
* **Precision Combat System**:
  * **`Q` Key — Dash**: 8-directional burst with **0.25s invulnerability frames (i-frames)** to phase safely through attacks and projectiles (`EVADED`).
  * **`F` Key — Timed Parry**: Active guard window with radiant cyan-gold shield barrier. Deflecting melee blows inflicts **STAGGERED!** on enemies and grants a **+50% (1.5x) Counter-Attack Damage Buff**.
  * **Projectile Deflection**: Parrying incoming enemy spells and arrows reflects them back toward your cursor with amplified speed and damage.
* **Will Stat & Stagger Mechanics**:
  * Both player and enemies possess a **Will** stat mitigating stun and stagger duration from devastating blows:
    ```text
    Stun Duration = Base Stun * 100 / (100 + Will * 1.25)
    ```
  * Higher Will guarantees faster posture recovery (e.g. Unarmored 20 Will = 0.80s stumble; Crusader Plate 65 Will = 0.55s).
  * **Anti-Stunlock Grace Buffer**: Recovering from a stagger grants **1.8s of Stagger Immunity**, preventing unfair chain-stuns.
* **Consumable Health Potions**:
  * Carry consumable health flasks across your journey (`[1]` or `[H]` to drink).
  * Restores **+45 HP** upon consumption with a **7.0-second cooldown timer**.
* **Out-of-Combat Health Regeneration**:
  * Remaining disengaged from combat for 4+ seconds smoothly regenerates health at **+8.0 HP/s** up to maximum capacity.
* **Equipment & Stat Scaling (Armor & Totems)**:
  * Collectible armor sets with dynamic damage mitigation:
    ```text
    Damage = Raw Damage * [ 100 / (100 + Defense * (1 - Pierce)) ]
    ```
  * Equippable sacred totems that alter stamina recovery rates, attack power, Will resilience, and cooldown timings.
* **Paginated Multi-Page Dialogue & Lore Codex**:
  * Clean, interactive dialogue box supporting multi-page conversations with Previous (`[A]`), Next (`[D]`), and Close (`[ESC]`) navigation.
  * Comprehensive in-game Lore Journal (`[I]` / `[TAB]`) tracking 10 uncoverable historical fragments across the realm with unread notification indicators.
* **Collapsible Objectives Tracker (`[O]`) & Location Banner**:
  * Dynamic top HUD displaying active environment (`RUINED CASTLE`, `CROSSROADS`, `ZYPHDEN`, `HEARTHSIDE TAVERN`, `ZYPHDEN ROYAL KEEP`, `ARENA`).
  * Collapsible quest card pinned cleanly to the middle-left margin.
* **Procedural Medieval Acoustic Audio**:
  * 100% custom-synthesized medieval soundscape with zero retro/sci-fi synth beeps:
    - Cathedral abbey bells with acoustic overtone partials.
    - Tempered steel blade cutting swooshes.
    - Anvil & broadsword clashes with metallic ringing.
    - Chainmail & plate armor blunt impact thuds.
    - Crossbow string releases with wooden timber creaks.
    - Deep medieval war horns and portcullis iron chain rumbles.
* **Combat Arena & Wave Defense Mode - 5 Stages**:
  * 5 escalating combat waves featuring mixed vanguard formations, high-threat charger vanguards, dual-brute shockwaves, kiting snipers, and culminating in **The Grand Inquisitor** boss trial on Wave 5.
  * **Adhesive Bandages +30 HP**: Glowing medical pickups spawn periodically across the arena floor and drop upon defeating enemies, providing tactical healing sustain during prolonged trials.
  * **Combat Arena Unlock**: Unlocked from the Title Screen and Lobby upon completing the current storyline progress (claiming the Crusader Plate).

---

## Controls

| Action | Key / Input | Gameplay Role |
| :--- | :--- | :--- |
| **Movement** | `W, A, S, D` or `Arrow Keys` | 8-directional fluid traversal with solid wall, counter, and pillar collisions. |
| **Aiming** | `Mouse Cursor` | Reticle targeting for sword thrusts, dashes, and parry reflections. |
| **Light Attack** | `Left Mouse Button` | Swift slash/thrust. Low/zero stamina cost, rapid recovery cadence. |
| **Heavy Cleave** | `Right Mouse Button` | High-damage signature moves (multi-stage attacks, lunges, shockwaves). |
| **Dash (Evade)** | `Q Key` | Fast directional burst with **i-frames** (`EVADED`) to dodge sweeps & missiles. |
| **Timed Parry** | `F Key` | Active guard. Staggers melee foes and **reflects incoming projectiles**. |
| **Health Potion** | `1 Key` or `H Key` | Consumes 1 potion to restore **+45 HP** (triggers **7.0s cooldown**). |
| **Interact / Advance** | `E Key` / `Space` / `Enter` | Talk to NPCs, claim weapons/armor, inspect lecterns, and transition doors. |
| **Dialogue Navigation** | `A Key` / `D Key` | Navigate between previous and next pages of active dialogue. |
| **Dismiss / Close** | `ESC Key` | Safely close active dialogue boxes, pop-ups, and modals without closing the game. |
| **Toggle Objectives** | `O Key` | Expand or collapse the Chapter 1 objectives tracking card. |
| **Lore Journal & Inventory** | `I Key` or `TAB` | Toggle the in-game chronicles codex, paper doll, armor stats, and totems. |
| **Controls Guide** | `C Key` | Toggle the quick-reference controls overlay modal. |
| **Title Screen** | `~ Key` (Tilde / Grave) | Instantly return to the Title Screen from any map or combat state. |
| **Restart / Respawn** | `R Key` | Instantly respawn after defeat or restart the combat trial. |
| **Return to Lobby** | `L Key` | Return to the Armory Sanctuary from lobby-connected states. |

---

## Weapon Arsenal

| Weapon | Stance Style | Light Atk | Heavy Atk | Dash Cooldown | Passives & Special Properties |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Unarmed** | Bare Fists | None | None | 0.75s (15 stam) | Starting tutorial state. Search the courtyard altar to claim a weapon. |
| **Crusader's Broadsword** | Balanced Steel | 18 dmg (110° arc, 15 stam) | 70 dmg (2-Stage cleave) | 0.85s (20 stam) | Dependable reach. **Heavy**: Stage 1 wide 160° cleave (42 dmg) followed by Stage 2 upward shock slash line/box AoE (28 dmg, 2.1s cd). |
| **Truth Seeker (Partial)** | Holy Blade Fragment | 26 / 24 dmg (2-Hit Combo) | 78 dmg (Dual 360° + Holy Beam) | 0.65s (16 stam) | **Light**: Slash into radiant upward slice (line/box AoE). **Heavy**: Rapid dual 360° dawnfire sweeps (42 + 36 dmg) + holy beam projectile (22 dmg, 10% pierce, 3.8s cd). |
| **Dual Daggers** | Agility Flurry | 11 dmg (twin slice, **0 stam**) | 60 dmg (Lunge + Instant Spin) | 0.50s (14 stam) | Zero stamina light attacks. **Heavy**: High-velocity piercing lunge (**22 dmg + i-frames**) into instant 360° whirlwind (**38 dmg**, 95px range, 1.8s cd). |
| **War Mace** | Crushing Poise | 28 dmg (Frontal Circle Smack) | 96 dmg (Earthshaker 2-Stage) | 1.25s (26 stam) | High damage bludgeon. **Light**: Frontal circular smack. **Heavy**: Amplified frontal smack (52 dmg) followed by 360° tremor shockwave (44 dmg, **ignores 20% enemy defense**, 3.6s cd). |

> [!NOTE]
> In **Chapter 1: Ruined Castle**, the player begins unarmed with zero weapons in inventory. The Crusader's Broadsword is claimed at the central altar plinth, and all **4 Lore Fragments** are hidden as interactive world artifacts across the castle ruins:
> 1. **Fragment I: The Great Unification** (`frag_01`): Ancient Chronicle Stele near the southern Awakening Courtyard.
> 2. **Fragment II: The Shadow over Zyphden** (`frag_02`): Ancient Relic Chest in the eastern alcove across the stone bridge.
> 3. **Fragment III: Zil's Holy Oath** (`frag_03`): Sun-Crusader Inscribed Relic in the western ruins (and awarded upon defeating Zil's Fragment).
> 4. **Fragment IV: The Five Broken Seals** (`frag_04`): Royal Obsidian Reliquary in the northwest library alcove.
> 
> The High Altar is sealed by a **Sanctuary Ward** that blocks access to the upper dais until the first tutorial enemy (**Ruin Sentinel**) is officially slain. After defeating the sentinel and conquering **Zil's Fragment** (wielding 1% of Zil's true power with Truth Seeker's attack pattern), the legendary **Truth Seeker (Partial)** is retained permanently in your arsenal.

---

## Enemies & Boss Encounters

| Enemy | Archetype | Defense | Telegraph Shape | Combat Behavior & Defeat Strategy |
| :--- | :--- | :--- | :--- | :--- |
| **Cultist Slasher** | Melee Vanguard | 8 | Amber/Red Cone (95°, 115px) | Forward blade slash. Sidestep or time `[F]` right before impact to stagger. |
| **Deathseeker Archer** | Ranged Kiting | 6 | Red Laser Aim Line (360px range) | Backs away if approached; fires high-velocity piercing bolts. |
| **Void Pyromancer** | Occult Caster | 10 | 3-Way Trajectory & Landing Tinges (320px) | Displays exact trajectory lines and glowing landing tinges that flare bright right before casting void orbs. Evade with `[Q]` or reflect with `[F]`. |
| **Shadow Charger** | Dash Melee | 12 | Linear Path (50px width, 240px) | High-speed dash thrust. Step perpendicular to path or parry head-on. |
| **The Grand Inquisitor** | **Arena Boss** | 26 | Multi-Phase Trajectories & Tremor AoE | 500 HP boss rotating between **12-way Radial Star Trajectories**, **Linear Beam Shard**, and **Cataclysmic Smite AoE**. |
| **Zil's Fragment** | **Chapter 1 Boss (1% Power)** | 28 | Line / Box AoE, 360° Circle & Holy Beam Line | 380 HP remnant possessing 1% of Zil's true power. Faithfully utilizes Truth Seeker's moveset: Upward Radiant Box Slice, Dawnfire 360 Tremor Sweep, and Holy Beam piercing blast. |
| **Iron Brute** | Heavy Crusher | 32 | Expanding Circle (150px AoE) | High armor tank. Tremor ground slam. Dash clear with `[Q]` or pierce armor with War Mace Earthshaker. |

> [!TIP]
> **Visual Clarity Rule**: Filled Cone AoEs are reserved strictly for attacks that deal direct melee/cleave damage within that sector. Projectiles display subtle trajectory guide lines and a glowing impact landing tinge that intensifies as the attack is about to be casted, allowing clean precision dodging.

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
