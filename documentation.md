# PHLEGMATIC — Complete Game Documentation & Technical Reference

Welcome to the comprehensive technical and design manual for **Phlegmatic**, a dark fantasy medieval top-down action RPG built in C++17 with Raylib 6.2.

---

## Table of Contents
1. [Overview & Lore of Tydeshire](#1-overview--lore-of-tydeshire)
   - [The Great Unification & The Five Crowns](#the-great-unification--the-five-crowns)
   - [The Fracture & The Long Road](#the-fracture--the-long-road)
   - [Lore Fragments Codex (I–X)](#lore-fragments-codex-ix)
   - [Characters & Royal Court](#characters--royal-court)
2. [Campaign Progression & World Design](#2-campaign-progression--world-design)
   - [Act I: The Ruined Castle](#act-i-the-ruined-castle)
   - [Act II: The Crossroads of the Damned](#act-ii-the-crossroads-of-the-damned)
   - [Realm Transition Screen](#realm-transition-screen)
   - [Act III: Fortified Realm of Zyphden](#act-iii-fortified-realm-of-zyphden)
   - [Hearthside Tavern](#hearthside-tavern)
   - [Expanded Zyphden Royal Keep](#expanded-zyphden-royal-keep)
   - [Combat Arena & Endless Wave Defense](#combat-arena--endless-wave-defense)
3. [Combat Systems & Mathematical Formulas](#3-combat-systems--mathematical-formulas)
   - [Defense & Armor Penetration Formula](#defense--armor-penetration-formula)
   - [Will Stat & Stagger Duration Formula](#will-stat--stagger-duration-formula)
   - [Anti-Stunlock Buffer](#anti-stunlock-buffer)
   - [Invulnerability Frames (i-frames)](#invulnerability-frames-i-frames)
   - [Parry Guard, Counter Buff & Projectile Reflection](#parry-guard-counter-buff--projectile-reflection)
   - [Consumable Health Elixirs & Out-of-Combat Regeneration](#consumable-health-elixirs--out-of-combat-regeneration)
4. [Controls & Interface Reference](#4-controls--interface-reference)
   - [Keybindings Guide](#keybindings-guide)
   - [Modular Visual Action Bar](#modular-visual-action-bar)
   - [Attributes & Posture Status Indicators](#attributes--posture-status-indicators)
   - [In-Game Lore Journal & Equipment Paper Doll](#in-game-lore-journal--equipment-paper-doll)
5. [Weapon Arsenal & Combat Stances](#5-weapon-arsenal--combat-stances)
   - [Crusader's Broadsword](#crusaders-broadsword)
   - [Truth Seeker (Partial)](#truth-seeker-partial)
   - [Dual Daggers](#dual-daggers)
   - [War Mace](#war-mace)
6. [Bestiary & Boss Encounters](#6-bestiary--boss-encounters)
   - [Standard Hostiles](#standard-hostiles)
   - [Zil's Remnant (Shadow of the Sun Crusader)](#zils-remnant-shadow-of-the-sun-crusader)
   - [Knight Lenon (Vanguard Captain Spar)](#knight-lenon-vanguard-captain-spar)
   - [The Grand Inquisitor (Arena Boss)](#the-grand-inquisitor-arena-boss)
7. [Technical Architecture & Engine Pipeline](#7-technical-architecture--engine-pipeline)
   - [Directory Structure](#directory-structure)
   - [State Coordinator Pattern](#state-coordinator-pattern)
   - [Collision & Hit Registration](#collision--hit-registration)
   - [Procedural Audio Synthesis](#procedural-audio-synthesis)
   - [Custom Asset Drop Pipeline](#custom-asset-drop-pipeline)
8. [Building & Setup Guide](#8-building--setup-guide)
   - [Automated MinGW-w64 Batch Script](#automated-mingw-w64-batch-script)
   - [CMake & Ninja Build](#cmake--ninja-build)
   - [Precompiled Executables](#precompiled-executables)

---

## 1. Overview & Lore of Tydeshire

### The Great Unification & The Five Crowns
For more than five centuries, the Land of Tydeshire was consumed by unrelenting feudal warfare. Warlords clashed over barren frontiers, and regional dynasties collapsed as quickly as they arose. Unity remained impossible until **King Charlz** ascended through martial supremacy and iron will, forging an empire from the blood of five fractured territories. 

Crowned *Charlz the Liege of Kings*, he divided the continent into five sovereign realms to govern peace:
1. **Zyphden** — Realm of the Gale Winds and high wind-spires.
2. **Azshden** — Iron cliffs and coastal bastions.
3. **Fleurshden** — Fertile river valleys and blooming thorns.
4. **Noirden** — Shrouded obsidian peaks and mists.
5. **Solsden** — Radiant sun-plains, golden altars, and the High Sanctuary.

### The Fracture & The Long Road
The Pax Charlz endured for three decades. But peace in Tydeshire was only ever borrowed. King Charlz succumbed to an unknown creeping madness, dissolving royal assemblies and vanishing into the northern frontiers without an heir. With the High Throne vacant, fear turned brother against brother. Border seals were fractured, and primeval monstrosities crawled forth from the mire.

The player begins their pilgrimage amidst the cold ashes of the southern sanctuary, armed with little more than instinct, bound on a long trek north through the corrupted **Crossroads** into the fortified gates of **Zyphden**.

### Lore Fragments Codex (I–X)
The game world contains ten discoverable historical fragments documented inside the in-game Journal (`[I]` or `[TAB]`):
* **Fragment I: The Great Unification (`frag_01`)**: The founding covenant of King Charlz and the five crowns. Uncovered at the southern Awakening Courtyard Stele.
* **Fragment II: The Shadow over Zyphden (`frag_02`)**: Early rumors of border anomalies and void creatures. Discovered in the eastern ruins relic chest and the Zyphden noticeboard.
* **Fragment III: Zil's Holy Oath (`frag_03`)**: The sworn vow of Sun-Crusader Zil as he marched south to anchor the high altars. Awarded upon overcoming Zil's Remnant.
* **Fragment IV: The Five Broken Seals (`frag_04`)**: Historical breakdown of the elemental warding seals that once held back primeval corruption. Found in northwest library ruins.
* **Fragment V: The Crossroads Watch (`frag_05`)**: The tragic massacre of the southern border patrol when the highway went dark. Uncovered at the Crossroads shrine.
* **Fragment VI: Rite of the Gale Winds (`frag_06`)**: Architectural folios describing the resonance leylines powering Zyphden's defensive ramparts. Found at the Wind Lectern in Zyphden.
* **Fragment VII: Lenon's Field Log (`frag_07`)**: Personal field journal entries of Vanguard Captain Lenon mourning the disappearance of his sworn brother Zil. Found near the eastern courtyard gates.
* **Fragment VIII: The Sovereign's Burden (`frag_08`)**: Royal decrees by King Kreptu authorizing grain rationing and martial conscription. Uncovered at the royal noticeboard.
* **Fragment IX: Inscription of the Ancient Pillars (`frag_09`)**: Runes carved by the First Wardens explaining how primeval monoliths steady the soul's willpower against posture damage. Discovered by inspecting Inscribed Pillars.
* **Fragment X: The Royal Armory Archives (`frag_10`)**: The tactical compendium detailing armor piercing, stagger damage multipliers, and counter-riposte disciplines. Found inside the Royal Keep Armory Relic Chest.

### Characters & Royal Court
* **King Kreptu (Monarch of Zyphden)**: Sovereign lord presiding over the gale realm from his golden dais. Pragmatic, weary, yet resolute, he sanctions courtyard combat trials for worthy champions.
* **Knight Lenon (Vanguard Captain)**: Sworn brother of Lord Zil. An honorable martial master wielding high-velocity lunges and whirlwind cleaves in the Eastern Courtyard.
* **Zil's Remnant (Shadow of the Sun Crusader)**: The spectral guardian of the high altar in the Ruined Castle, wielding Truth Seeker to test the conviction of wanderers.
* **Chancellor Malakor**: Head of the Zyphden Privy Council. Cynical and calculating, he manages tax revenues and mystical leylines.
* **Sentinel Donald & Sentinel Garrick**: Twin Royal Halberdiers guarding the sovereign throne dais with unyielding martial vigilance.
* **Archivist Thaddeus**: Keeper of ancient chronicles in the Western Archives. Reverent scholar of ancient monoliths and elemental leylines.
* **Lady Genevieve**: Noble patron of the Royal Armory who advises on armor craft and weapon balance.
* **Tavernkeep Eldon**: Hospitable barkeep running the Hearthside Tavern, serving warm stew and shelter by the hearth.
* **Chronicler Cedric, Blacksmith Varek, Elder Maeve**: Town residents offering historical wisdom, armor-crafting guidance, and refugee perspectives.

---

## 2. Campaign Progression & World Design

### Act I: The Ruined Castle
The player awakes unarmed in an overgrown cathedral courtyard.
1. Inspect the central plinth to claim the **Crusader's Broadsword**.
2. Master movement (`[WASD]`), directional dashing (`[Q]`), parrying (`[F]`), and light/heavy sword slashes.
3. Slay the guardian Ruin Sentinel to dissolve the magical **Sanctuary Ward** sealing the high altar.
4. Duel **Zil's Remnant** at the high dais:
   - Defeating him awards the sacred **Truth Seeker (Partial)** and **Fragment III**.
   - If defeated, the player is granted an honorable rematch modal or can accept his mercy to receive **Zil's Token**.
5. Pass through the northern gateway to enter the Crossroads.

### Act II: The Crossroads of the Damned
A ruined wilderness connecting the southern ruins to the northern fortress.
* High-threat monster ambushes (Cultist Slashers, Deathseeker Archers, Void Pyromancers, and Shadow Chargers).
* Ancient shrine holding the **Totem of Sanity** (+15% Stamina Regen, +10 Will).
* Highway Inscribed Monolith (`frag_09`) and Crossroads Supply Chest (+2 Health Potions).
* Northern gateway leading into the Zyphden transition portal.

### Realm Transition Screen
An atmospheric realm-entry screen bridging the wilderness to Zyphden:
* Elegant dark stone presentation card with ambient dust particles and corner brackets.
* Automatic asset drop slot (`assets/zyphden_transition.png`): If present, displays custom 16:9 illustration; otherwise renders a blueprint placeholder with dimension guides.
* Press `[SPACE]`, `[ENTER]`, or click to step across the threshold into Zyphden.

### Act III: Fortified Realm of Zyphden
The capital city of the gale winds:
* Cobblestone avenues, residential manors, market squares, and royal gardens.
* Town noticeboard and three lore historians (Cedric, Varek, Maeve).
* Three environmental lore items (`frag_06`, `frag_07`, `frag_08`).

### Hearthside Tavern
Cozy indoor retreat with a roaring fireplace, timber bar, and warm acoustics:
* Meet **Tavernkeep Eldon** behind the counter to initiate the royal questline.
* Triggers scripted entrance of **Knight Lenon** entering through the tavern doorway to inspect your blade or Zil's token.

### Expanded Zyphden Royal Keep
Grand stone hall expanded to **1200x1100** bounds:
* Stepped royal dais with King Kreptu seated upon his golden throne at `{ 1000, 275 }`.
* 940px crimson runner carpet leading from the southern entry portal directly to the throne, marked with the gold Sovereign Seal medallion.
* Eight solid marble colonnade pillars, Western Royal Archives with bookshelves, and Eastern Armory with weapon displays.
* Five interactive court NPCs (Malakor, Donald, Garrick, Thaddeus, Genevieve).
* Inscribed Monolith (`frag_09`), Armory Chest (+2 Potions, `frag_10`), and Provisions Chest (+1 Potion).

### Combat Arena & Endless Wave Defense
Unlocked permanently upon completing the Chapter 1 storyline:
* Accessible directly from the Title Screen or the Lobby portal.
* 5 escalating combat waves featuring mixed vanguard formations, dual-brute shockwaves, and the climactic boss encounter against **The Grand Inquisitor**.
* Adhesive Bandages (+30 HP) spawn periodically across the arena floor for sustained survival.

---

## 3. Combat Systems & Mathematical Formulas

### Defense & Armor Penetration Formula
All damage inflicted upon the player or enemies undergoes damage reduction via the mitigation equation:

```text
Mitigated Damage = Raw Damage * [ 100 / (100 + Defense * (1 - Pierce)) ]
```

* **Defense**: Physical defense rating provided by equipment (e.g. Crusader Plate = 24 Def) or enemy archetype.
* **Pierce**: Armor penetration fraction (e.g. War Mace heavy tremors ignore 20% of target defense, `Pierce = 0.20`).
* **Minimum Damage**: Unmitigated blows will always deal at least 1 damage.

### Will Stat & Stagger Duration Formula
When an entity is parried or struck by devastating heavy blows (e.g. Iron Brute slams, Charger thrusts, boss smites), they enter a **Staggered** state where actions are disabled:

```text
Stagger Duration = Base Stagger * 100 / (100 + Will * 1.25)
```

* **Player Will Scaling**:
  - Unarmored (Tutorial): 20 Will -> **0.80s** stagger.
  - Crusader Plate: 65 Will -> **0.55s** stagger.
* **Enemy Will Scaling**:
  - Cultist Slasher (10 Will) -> **1.95s** stagger upon parry.
  - Deathseeker Archer (15 Will) -> **1.85s** stagger upon parry.
  - Void Pyromancer (20 Will) -> **1.76s** stagger upon parry.
  - Shadow Charger (30 Will) -> **1.60s** stagger upon parry.
  - Iron Brute (50 Will) -> **1.35s** stagger upon parry.
  - The Grand Inquisitor (60 Will) -> **1.25s** stagger upon parry.
  - Zil's Remnant (65 Will) -> **1.21s** stagger upon parry.
  - Knight Lenon (70 Will) -> **1.17s** stagger upon parry.

### Anti-Stunlock Buffer
To ensure fair and responsive combat, recovering from a stagger immediately grants the player **1.8 seconds of Stagger Immunity**. During this grace window, heavy enemy blows will still deal damage but cannot chain-stagger the player.

### Invulnerability Frames (i-frames)
Activating **Dash (`[Q]`)** triggers a rapid 8-directional burst with **0.25 seconds of invulnerability frames**. During this active evasion window:
* Melee swings and shockwaves pass through the player harmlessly.
* Projectiles are safely phased through.
* Spawns an amber-white `"EVADED"` combat badge above the player's head.

### Parry Guard, Counter Buff & Projectile Reflection
Pressing **Parry (`[F]`)** activates a golden cyan-tinted shield barrier for **0.24 seconds**:
* **Melee Deflection**: Parrying a melee attack perfectly negates all damage, inflicts **STAGGERED!** on the attacker, and grants the player a **+50% (1.5x) Counter-Attack Damage Multiplier** for 2.4 seconds.
* **Projectile Reflection**: Parrying an incoming enemy arrow or spell projectile deflects it back toward your mouse reticle with double speed and amplified damage.

### Consumable Health Elixirs & Out-of-Combat Regeneration
* **Health Potions (`[1]` or `[H]`)**: Drinking an elixir restores **+45 HP** immediately and initiates a **7.0-second cooldown timer**.
* **Out-of-Combat Regeneration**: Staying disengaged from combat for **4.0+ seconds** activates passive health recovery at **+8.0 HP/s** up to maximum capacity.

---

## 4. Controls & Interface Reference

### Keybindings Guide

| Key / Input | Action Name | Description |
| :--- | :--- | :--- |
| `W, A, S, D` / `Arrows` | Movement | 8-directional traversal with solid wall collision. |
| `Mouse Cursor` | Aiming | Reticle aiming for sword thrusts, dashes, and parry reflections. |
| `Left Mouse Button` | Light Attack | Rapid slash/thrust with minimal recovery cadence. |
| `Right Mouse Button` | Heavy Attack | Signature multi-stage strike, lunge, or shockwave. |
| `Q Key` | Dash (Evade) | Fast directional burst with **0.25s i-frames** (`EVADED`). |
| `F Key` | Timed Parry | Active guard window. Staggers melee foes and reflects missiles. |
| `1 Key` or `H Key` | Health Potion | Consumes 1 elixir to heal **+45 HP** (**7.0s cooldown**). |
| `E Key` / `Space` / `Enter` | Interact | Talk to NPCs, open chests, read monoliths, and use doors. |
| `A Key` / `D Key` | Dialogue Page | Navigate between previous and next pages of active dialogue. |
| `ESC Key` | Close / Dismiss | Safely dismiss open dialogues, popups, and journal modals. |
| `O Key` | Toggle Objectives | Expand or collapse the pinned Chapter 1 quest tracker card. |
| `I Key` or `TAB` | Lore & Equipment | Open the in-game Journal, Equipment stats, and Totem slots. |
| `C Key` | Controls Guide | Toggle the quick-reference keybindings overlay. |
| `~ Key` (Tilde / Grave) | Title Screen | Instantly return to the Title Screen. |
| `R Key` | Respawn / Retry | Instantly restart an active trial or respawn after defeat. |
| `L Key` | Return to Lobby | Warp back to the Armory Sanctuary from lobby-connected states. |

### Modular Visual Action Bar
Located at the bottom center of the HUD, the visual action bar provides real-time ability tracking:
* **Slot 1 (`[Q]`)**: Dash — Icon, active evasion pulse, and recovery sweep.
* **Slot 2 (`[LMB]`)**: Light Attack — Cadence recovery wipe.
* **Slot 3 (`[RMB]`)**: Heavy Attack — Multi-stage cooldown sweep with numeric timer (`2.1s`, `3.6s`).
* **Slot 4 (`[F]`)**: Parry — Radiant gold when ready, cyan flash on active guard, dark recovery wipe.
* **Slot 5 (`[1]`)**: Health Flask — Emerald theme, stock badge (`xN`), and **7.0s countdown timer**.

### Attributes & Posture Status Indicators
Displayed directly alongside the Player Health and Stamina bars:
* **DEF: XX**: Total active defense rating including equipped armor.
* **WILL: XX**: Current willpower stat mitigating incoming stagger durations.
* **Dynamic Posture Pill**: Displays real-time state: `STAGGERED!`, `POISE GUARD`, `I-FRAMES`, `+50% COUNTER`, or `COMBAT READY`.

### In-Game Lore Journal & Equipment Paper Doll
Opened via `[I]` or `[TAB]`:
* **Tab 1: Equipment & Arsenal**: Inspect weapons, armor sets, damage mitigation statistics, and equip discovered sacred totems.
* **Tab 2: Lore Journal**: Browse 10 discovered historical fragments with unread badges (`(!)`).

---

## 5. Weapon Arsenal & Combat Stances

| Weapon | Light Attack | Heavy Cleave | Dash CD | Special Properties & Passives |
| :--- | :--- | :--- | :--- | :--- |
| **Unarmed** | None | None | 0.75s | Starting state in cathedral awakening. |
| **Crusader's Broadsword** | 18 dmg (110° arc) | 70 dmg (2-Stage cleave) | 0.85s | Balanced reach. Heavy cleave consists of Stage 1 wide 160° sweep (42 dmg) followed by Stage 2 upward line/box shockwave (28 dmg). |
| **Truth Seeker (Partial)** | 26 / 24 dmg (2-Hit) | 78 dmg (Dual 360° + Beam) | 0.65s | Consecrated relic blade. Light attack chains a slash into an upward radiant slice. Heavy unleashes dual 360° sweeps (42 + 36 dmg) and fires a holy piercing beam (22 dmg, 10% pierce). |
| **Dual Daggers** | 11 dmg (**0 Stamina**) | 60 dmg (Lunge + Spin) | 0.50s | Zero stamina light attacks. Heavy executes a high-velocity piercing lunge (**22 dmg + i-frames**) followed by a 360° whirlwind (**38 dmg**). |
| **War Mace** | 28 dmg (Frontal Smack) | 96 dmg (Earthshaker) | 1.25s | Massive poise crusher. Heavy executes an earth-shattering overhead smash followed by a 360° ground tremor that **ignores 20% of enemy defense**. |

---

## 6. Bestiary & Boss Encounters

| Enemy Name | Type | Def | Will | Telegraph Indicator | Combat Behavior |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Cultist Slasher** | Melee Vanguard | 8 | 10 | Amber/Red Cone (95°, 115px) | Swift forward slash. Sidestep or parry to stagger for 1.95s. |
| **Deathseeker Archer** | Ranged Kiting | 6 | 15 | Red Laser Aim Line (360px) | Retreats when approached; fires piercing bolts. Parry reflects bolt. |
| **Void Pyromancer** | Occult Caster | 10 | 20 | 3-Way Laser & Glowing Landing Tinges | Casts tri-directional void orbs. Orbs can be reflected back. |
| **Shadow Charger** | Dash Melee | 12 | 30 | Linear Path (50px width, 240px) | High-speed linear thrust. Evade laterally with `[Q]` or parry head-on. |
| **Iron Brute** | Heavy Crusher | 32 | 50 | Expanding Red Circle (150px AoE) | High armor tank. Ground slam inflicts heavy stagger if unevaded. |
| **Zil's Remnant** | **Chapter 1 Boss** | 28 | 65 | Box AoE, 360° Sweep & Beam Line | 520 HP boss utilizing Truth Seeker moveset. Features a telegraphed 0.42s follow-up cleave. |
| **Knight Lenon** | **Courtyard Boss** | 32 | 70 | Cone AoE & Whirlwind Ring | 640 HP vanguard captain. Dynamic combo cadence with high-speed lunges and spins. |
| **The Grand Inquisitor** | **Arena Boss** | 26 | 60 | 12-Way Radial Star & Beam Shard | 500 HP trial boss rotating between radial star salvos and smite shockwaves. |

---

## 7. Technical Architecture & Engine Pipeline

### Directory Structure
```
Phlegmatic/
├── assets/                     # Custom drop directory for sprites & transition illustrations
│   └── README.md               # Asset specifications (dimensions & formats)
├── bin/                        # Precompiled 64-bit Windows executable
├── src/
│   ├── main.cpp                # Game entry point & window lifecycle
│   ├── core/                   # Engine config, procedural sound, particle emitter
│   ├── combat/                 # Weapons, armor, collision, math formulas, projectiles
│   ├── world/                  # Level geometry, map environments, interactable monoliths
│   ├── entities/               # Player, enemy AI, modular vector renderers
│   ├── rendering/              # Procedural vector silhouettes & asset loader
│   └── states/                 # State coordinators, handlers, and modular HUD/UI
├── vendor/raylib/              # Precompiled Raylib 6.2 headers & static libraries
├── build.bat                   # Automated compilation batch script
├── run.bat                     # Launcher script
└── CMakeLists.txt              # Cross-platform CMake configuration
```

### State Coordinator Pattern
State coordination is decoupled into discrete state handlers:
* `RuinedCastleHandler`: Manages Act I tutorial, sanctuary ward, and Zil encounter.
* `CrossroadsHandler`: Manages Act II wilderness encounters, ambushes, and gateway triggers.
* `ZyphdenHandler`: Manages Act III town logic, tavern sequence, expanded Royal Keep, and courtyard duel.
* `WaveManager`: Controls Arena trial waves, spawning algorithms, and medical pickup timers.

### Collision & Hit Registration
`CollisionSystem.cpp` performs precision geometric checks:
* Sector / Cone checks via dot products and angular offsets for weapon sweeps.
* Oriented bounding boxes for beam attacks and lunges.
* Circle-to-circle hit registration for projectile deflections and proximity pickups.

### Procedural Audio Synthesis
All audio cues are procedurally synthesized at runtime via Raylib's audio buffer APIs (`SoundFX.cpp`), eliminating external sound asset dependencies:
* Cathedral abbey bells with acoustic overtone partials.
* Steel blade slashes and parry clangs with realistic metallic ring decay.
* Heavy plate armor blunt impact thuds.
* Wooden crossbow releases and portcullis gate rumbles.

### Custom Asset Drop Pipeline
Custom assets placed in `assets/` are automatically loaded at launch:
* `assets/zyphden_transition.png`: 1920x1080 or 1280x720 artwork replacing the Zyphden transition placeholder screen.
* `assets/player.png`: 64x64 or 128x128 sprite sheet replacing procedural player silhouette.
* `assets/enemy_melee.png`: Custom slasher enemy sprite.
* `assets/enemy_brute.png`: Custom brute crusher boss sprite.
* `assets/tileset.png`: Custom floor tiles replacing procedural stone flagstones.

---

## 8. Building & Setup Guide

### Automated MinGW-w64 Batch Script
Run the automated build script from PowerShell or Command Prompt:
```powershell
.\build.bat
```
This invokes GCC 17 with `-O2 -static -std=c++17`, linking static Raylib 6.2 libraries, and produces `phlegmatic.exe` (project root) and `bin\phlegmatic.exe`.

### CMake & Ninja Build
```powershell
cmake -B build -G Ninja
cmake --build build
.\build\phlegmatic.exe
```

### Precompiled Executables
Standalone executables are bundled directly in the repository:
* `.\phlegmatic.exe`
* `.\bin\phlegmatic.exe`
* Launch directly via `.\run.bat`.
