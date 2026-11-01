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
| **Return from Arena** | `T Key` | Teleport from Combat Arena back to Knight Lenon in Zyphden. |

---

## Current Version: 0.0.1e

> [!NOTE]
> Phlegmatic is currently on **Version 0.0.1e**.
>
> ### Version 0.0.1e Release Highlights
> * **Mini Ore Golem Companion Map Transition Synchronization & Leash Warping**:
  - Resolved the top-left { 0.0f, 0.0f } spawning bug when transitioning maps with the Elemental Core Totem equipped.
  - Implemented Player::RelocateCompanionToPlayer() which dynamically positions the loyal minion beside the player's flank/rear upon map transitions, teleportation, campfire fast travel, and save file loading.
  - Added a 350px leash warping safeguard in CollisionSystem that teleports separated or stuck companions directly back to the player with an elemental spark burst.
* **Centralized Game Constants (GameConstants.hpp) & Safe Save Helpers (SaveHelpers.hpp)**:
  - Eliminated all hardcoded magic numbers (8, 7, 6, 5, 9, 10) across save serialization, inventory queries, game states, and drawer loops, preventing array out-of-bounds access violations (SIGSEGV) and save data regressions.
  - Added robust JSON token parsing with whitespace trimming and exception-shielded numeric extractors (SafeParseInt, SafeParseFloat, SafeParseBool), protecting against malformed or legacy save file crashes.
* **Architectural Restructuring & Dedicated UI Subsystem (src/ui/)**:
  - Decluttered src/states/ by bundling all interface renderers and modals (HUD, ModalUI, TitleUI, UI) into a dedicated src/ui/ directory.
  - Separated helper parsers and game limit constants into src/core/, streamlining project maintainability.
* **Wind & Shadow Armaments (Gale Archer Signature Crafting)**:
  - Added two high-tier weapon archetypes forged from Gale Archer trophies (Shadow Essence) and Gale Bandit materials at Blacksmith Varek's Master Forge:
    - **Twin Zephyr Daggers**: High-velocity dual curved blades. Light attack unleashes an effortless wind flurry (0 stamina on hits 1-4, 10 on 5th hit). Heavy attack triggers *Shadow Slip*, an evasive 120px dash with 0.32s invulnerability frames that strikes in a 360-degree whirlwind (68 dmg) and leaves an explosive decoy.
    - **Windrunner's Recurve**: Phlegmatic's first player-usable ranged bow archetype. Light attack fires high-velocity piercing wind arrows across long range (24 dmg, 10 stamina). Heavy attack channels the *Gale Piercer Drill Arrow* (82 dmg) with 40% defense pierce and heavy poise knockback.
  - Full support in the Equipment modal quick-switch row and selection drawer, with dedicated 3D character doll hand-drawn vectors.
* **Campfire Waypoints & Realm Fast Travel**:
  - Erected interactive Campfire sites across all 5 major world realms:
    1. *Ruined Castle Courtyard Hearth* (`{ 780.0f, 650.0f }`)
    2. *Crossroads Highway Camp* (`{ 740.0f, 480.0f }`)
    3. *Zyphden Town Square Fire* (`{ 880.0f, 680.0f }`)
    4. *Subterranean Miners' Hearth* (`{ 960.0f, 660.0f }`)
    5. *Restless Glade Bonfire* (`{ 540.0f, 640.0f }`)
  - Interacting with any campfire (`[E] REST AT CAMPFIRE`) fully restores health and stamina, refills health potions to maximum (5 flasks), and permanently attunes the waypoint.
  - Opening the Realm Map (`[M]`) and selecting any attuned stage unlocks the **`[E] FAST TRAVEL TO CAMPFIRE`** action, initiating a seamless cinematic screen transition and teleporting the player directly beside the target campfire.
* **Humoral Bestiary & Tactical Lore Codex**:
  - Expanded the Lore Journal modal with a dedicated third section: **`[3] HUMORAL BESTIARY`**.
  - Features real-time kill tracking across all 9 enemy archetypes: Corrupted Slasher, Corrupted Archer, Void Pyromancer, Iron Brute, Grand Inquisitor, Ore Golem & Mini Golems, Elemental Golem Titan, Gale Bandit, and Gale Archer.
  - Displays humoral alignments, animated vector silhouettes, combat cadences, known weaknesses, and exact drop tables.
  - Features 3 progressive unlockable lore seals for each enemy: *Initiate Seal* (1 Kill), *Adept Seal* (5 Kills), and *Master Seal* (15 Kills).
* **Universal Screen Fade & Realm Transition System**:
  - Implemented a smooth cinematic transition system across all realm and map boundaries:
    - Zyphden <-> Restless Mist Forest
    - Zyphden <-> Royal Combat Arena
    - Zyphden <-> Subterranean Quarry Depths
    - Crossroads <-> Zyphden Realm
    - Ruined Castle <-> Crossroads
  - Transitions fade smoothly to full blackout (`ScreenFadeMode::FADE_OUT`, 0.35s), display a stylized gold and runic realm title card and subtitle during hold (`HOLD`, 0.25s) while executing map loading and player positioning at the midpoint, and then smoothly fade back into the destination landing screen (`FADE_IN`, 0.40s).
  - Input shielding during blackout prevents unintended player movement, skill activation, or combat damage during map transitions.
  - Fixes the Restless Mist Forest black screen bug by providing a complete 2D camera world render pass and combat update loop.
  - Smooth fade-in also accompanies save game loading, eliminating visual jarring or sudden screen snaps.
* **New Western Realm: Restless Mist Forest & Gale Archer Boss**:
>   - Journey through the western gates of Zyphden into the atmospheric **Restless Mist Forest** (1800x1300 woodland expanse).
>   - Encounter respawning **Gale Bandits**—agile dual-dagger skirmishers who throw swift daggers and drop Silver Shingles and **Essence of Mist** (35% drop rate; strictly NO Ore Scraps).
>   - Awaken the **Gale Archer Boss** at the ancient Altar of Whispers by offering 2x Essence of Mist (featuring 165px omni-directional interaction bounds, multi-key support [E/Space/Enter], live boss active detection, dynamic runic altar glow, and real-time essence count feedback).
>   - Defeat the Gale Archer across 3 telegraphed combat phases (Triple Windshot, Gale Piercer Beam, and Storm Rain Barrage) to earn guaranteed **Shadow Essence** and roll for the sacred **Totem of Mist** at an exact **15% drop rate** (strictly NO Ore Scraps).
> * **Shadowstrider Armor Set Crafting & Forge Pagination**:
>   - Blacksmith Varek in Zyphden now features **paginated armor crafting** under Tab 3 `[3] ARMOR FORGE (9)` (Page 1: Forged Scrap-Iron & Basalt Golem, Page 2: Shadowstrider) with clickable `[< PREV PAGE]` and `[NEXT PAGE >]` buttons and A/D / Arrow key navigation, eliminating UI overlap.
>   - Header now live-tracks all 4 crafting materials: Silver Shingles, Ore Scraps, **Essence of Mist**, and **Shadow Essence**.
>   - Strictly enforces component requirements for crafting Shadowstrider pieces: Cowl (2 Mist + 1 Shadow + 30 Shingles), Doublet (4 Mist + 2 Shadow + 50 Shingles), and Tights (3 Mist + 1 Shadow + 40 Shingles); pieces cannot be forged with shingles alone.
> * **Western Access Gate to Restless Mist Forest**:
>   - Extended Zyphden Realm western bounds to X: 100 and paved a cobblestone/moss pathway through the western alleyway (between Workshop and Tavern) connecting Town Square directly to the **Western Mist Archway** at `{ 160.0f, 730.0f }`.
>   - Features an ethereal glowing cyan portal, road signpost, and interactive floating prompt `[E] ENTER RESTLESS MIST FOREST`.
>   - Blacksmith Varek in Zyphden can now craft the agility-focused **Shadowstrider Set** (Cowl, Doublet, Tights) in Tab 3 using Essence of Mist, Shadow Essence, and Silver Shingles.
>   - Full 3/3 Set Bonus *Veil of the Mist* grants +25% Move Speed, -50% Dash Cooldown, +35% Dash Distance, -30% Dash Stamina Cost, +40% extended Dodge i-frames, and the active signature skill **Shadow Decoy & Vanish** ([V] key, 25s CD, 0.5s i-frames, creates an aggro-drawing illusion decoy that explodes for 40 shadow dmg).
> * **Royal Combat Arena 20-Wave Expansion & Difficulty Curve**:
>   - Expanded Arena from 5 waves to **20 escalating waves** featuring advanced humoral synergies across all enemy archetypes.
>   - Major boss encounters scheduled every 5 waves:
>     - **Wave 5**: *The Grand Inquisitor* + Pyromancers + Brute
>     - **Wave 10**: *Zil's Remnant* (Truth Seeker duel) + Chargers + Archers
>     - **Wave 15**: *The Gale Archer* + Gale Bandits + Chargers
>     - **Wave 20**: *The Primordial Elemental Golem Titan* + Mini Golems + Pyromancers
> * **Knight Lenon Arena Gating & Sanctuary Teleport Gateway**:
>   - Removed the direct Combat Arena shortcut button from the title screen.
>   - Arena is now exclusively accessible within the campaign by speaking with Knight Lenon in Zyphden Courtyard after proving your worth by defeating him in the sparring duel (`[R] ENTER ARENA (20 WAVES)`).
>   - An active **Exit Sanctuary Portal** at `{ 1200.0f, 520.0f }` and quick shortcut key **`[T]`** allow seamless teleportation from the Arena back to Knight Lenon in Zyphden at any time without losing campaign inventory or progression.
> * **Equipment HUD Overhaul & Text Word-Wrapping**:
>   - Expanded equipment modal to a spacious 1560x860 canvas with diamond loadout badges (`[ I ]`, `[ II ]`, `[ III ]`), large framed slots for modular armor, weapons, single totem relic, and material inventory bar (Potions, Shingles, Ore Scraps, Mist Essence, Shadow Essence).
>   - Right side features a 1.75x Hero showcase paper doll upon a glowing runic dais accompanied by live aggregate combat attributes (HP, STAM, DEF, WILL, SPEED, STAM REGEN).
>   - Implemented strict word wrapping on item drawer cards and set bonus descriptions via `FontManager::DrawTextWrapped` to eliminate horizontal text clipping across all display resolutions.
>   - Resolved crash when opening inventory modal: Fixed out-of-bounds loop indices in `ModalUI.cpp` for drawer category tabs (`slotCatNames[5]`) and totem relics (`totems[5]`), restricting iterations strictly to 5 elements.
> * **Modular 3-Piece Armor System & Full Set Bonuses**:
>   - Replaced monolithic suits with 3 modular equippable slots: **Helmet**, **Chestplate**, and **Leggings** (strictly NO boots, gloves, or rings).
>   - Equipping a matching 3/3 set activates unique passive stat bonuses and an active signature skill triggered by the **`[V]`** key:
>     - **Commoner's Scrap**: *Scrapper's Tenacity* (+15% Stamina Regen, +15% Shingle/Ore drops) | Skill: *Desperate Dust Burst* (30s CD, 180px 1.5s stun AoE, +35% move speed for 4s).
>     - **Crusader Plate**: *Aegis of the Sun* (+20% Defense, +25 Will) | Skill: *Dawnfire Aegis* (35s CD, 60 HP radiant barrier absorbing all strikes with holy detonation).
>     - **Vanguard Bastion**: *Unstoppable Juggernaut* (+40 Max HP, hyper-armor micro-stagger immunity during heavy attacks) | Skill: *Seismic Quake Slam* (22s CD, 200px 50 dmg shockwave, -25% armor shred, knocks foes flat).
>     - **Shadowstrider**: *Veil of the Mist* (+25% Move Speed, -50% Dash Cooldown, +35% Dash Distance, -30% Dash Stamina Cost, & +40% extended Dodge i-frames) | Skill: *Shadow Decoy & Vanish* (25s CD, 0.5s i-frames, leaves illusion decoy drawing enemy aggro, explodes for 40 shadow dmg).
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
>   - Automatically spawns a friendly Mini Elemental Golem companion with **75% stats** of the boss's minion (240 HP, 18 DMG, 28 DEF).
>   - Companion follows the player, attacks hostile enemies, and absorbs damage.
>   - If shattered, companion enters a **60-second respawn timer**, tracked in real time via a dedicated HUD Companion Status Widget above the player card.
>   - Light attack projectile upgraded to **infinitely piercing**, damage increased to **54.0**, and cooldown reduced to **3.0s**.
> * **Save File Modular Armor & Totem Persistence**:
>   - Crafted and unlocked modular armor pieces (`equippedHelmet`, `equippedChestplate`, `equippedLeggings`, `unlockedHelmets`, `unlockedChestplates`, `unlockedLeggings`, `unlockedTotems`) are now fully serialized into save slots (`save_slot_*.json`) and restored upon loading, preventing crafted gear loss.
> * **Ore Golem Leash & Return AI**:
>   - Relocated Ore Golem spawners into open quarry clearings away from colliders. Golems pursue targets within 320px and automatically de-aggro and walk back to their patrol spawn when lured beyond 420px.
> * **NPC Dialogue Game Pause**:
>   - Conversing with NPCs or inspecting monoliths now safely pauses player physics, enemy AI, projectiles, and combat collisions, granting complete immunity from damage during dialogue.
> * **Responsive UI & Text Enlargement**:
>   - Dynamically scales font sizes and bounding containers across both windowed (1280x720) and fullscreen (1080p+) resolutions, ensuring text never overflows or clips dialogue boxes, banners, or inventory panels.
> * **Boss Minion Summon Fix & Mini Golem Tankiness**:
>   - Elemental Golem boss now summons Mini Elemental Golems on an active 22s cooldown and immediately upon awakening. Mini Golems are buffed to 320 HP and 38 DEF to avoid being 1-shot and perform a telegraphed long-range Elemental Bolt snipe every 7 seconds.
> * **Weapon Heavy Attack Combo Revamp & Damage Rebalancing**:
>   - **Dual Daggers** (12.0 light / 58.0 heavy): 3-stage combo — *Shadow Puncture* (18 dmg, 0.2s iframes, applies 2.5s Bleed) -> *Backstab Cross-Slash* (28 dmg, +40% bonus against bleeding/rear targets) -> *Vault Kick* (12 dmg, 100% poise stagger, backward leap impulse).
>   - **War Mace** (24.0 light / 78.0 heavy): 2-stage combo — *Overhead Skull Crusher* (42 dmg, hyper-armor, inflicts -20% Armor Fracture debuff for 4s) -> *Seismic Tremor* (36 dmg, 360° circular shockwave, 25% defense pierce, knocks foes flat).
>   - **Crusader Blade** (18.0 light / 88.0 heavy): 3-stage combo — *Crescent Cleave* (30 dmg, restores +12 Composure) -> *Shock Thrust* (24 dmg, piercing line interrupting enemy windups) -> *Radiant Execution* (34 dmg, downward holy cleave dealing +50% bonus damage to staggered foes).
>   - **Truth Seeker** (22.0 light / 72.0 heavy): 2-stage combo — *Dawnfire Ring* (34 dmg, 360° sweep applying Solar Burn & firing holy beam) -> *Sacred Judgment Pillar* (38 dmg, descending sunray beam with 100% defense pierce that cleanses player debuffs).
>   - **Royal Halberd** (16.0 light / 28.0 heavy): 3-stage combo — *Long Lunge* with iframes -> *Sweeping Cleave* (220° arc) -> *Flurry* of 3 consecutive rapid stabs (30% weapon damage each).
> 
> * **Restless Mist Forest, Gale Archer Boss & Altar of Whispers**:
>   - Journey through the Western Mist Archway in Zyphden into the atmospheric woodland of the Restless Mist Forest.
>   - Defeat respawning Gale Bandits (35% Essence of Mist drop rate, zero Ore Scraps) and offer 2x Essence of Mist at the Altar of Whispers to summon the Gale Archer Boss.
>   - Gale Archer drops Shadow Essence and a 15% drop rate for the Totem of Mist.
> * **Shadowstrider Armor Set & Paginated Forge**:
>   - Craft the high-agility Shadowstrider set (Cowl, Doublet, Tights) at Blacksmith Varek's paginated armor forge using Essence of Mist, Shadow Essence, and Shingles.
>   - Full set bonus grants +25% Move Speed, -50% Dash Cooldown, +35% Dash Distance, -30% Dash Stamina Cost, +40% extended dodge i-frames, and the active [V] skill Shadow Decoy.
> * **Zephyr Daggers & Windrunner Recurve Armaments**:
>   - Twin Zephyr Daggers feature rapid mist slashes and the Shadow Slip whirlwind burst.
>   - Windrunner Recurve introduces ranged archery with piercing Gale Arrows and the infinite-penetration Gale Piercer Drill Arrow.
> * **Campfire Waypoints, Realm Fast Travel & Humoral Bestiary**:
>   - Rest at bonfires across all 5 realms to restore vitality, replenish elixirs, and attune fast-travel waypoints on the realm map modal (`[M]`).
>   - Track kills across all 9 enemy archetypes in the 3-tier Humoral Bestiary lore grimoire.
> * **Engine Resilience & Crash Stabilization**:
>   - Fixed save array overflow during state transitions, unified totem and campfire serialization, repaired projectile and weapon rendering pipelines, and added smooth fade transitions across all map boundaries.
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
