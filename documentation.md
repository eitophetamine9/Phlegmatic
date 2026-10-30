# PHLEGMATIC — Complete Game Documentation & Technical Reference



<div align="center">



[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)

[![Raylib](https://img.shields.io/badge/Engine-Raylib%20v6.2-white?style=for-the-badge&logo=raylib&logoColor=black&color=white)](https://www.raylib.com/)

[![Platform](https://img.shields.io/badge/Platform-Windows%20(x64)-0078D6?style=for-the-badge&logo=windows&logoColor=white)](https://www.microsoft.com/windows)

[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](LICENSE)



*The definitive technical reference manual, game system compendium, combat mathematics handbook, and narrative lore guide for **Phlegmatic**.*



---



</div>



## Table of Contents

- [Release Highlights: Version 0.0.1d](#release-highlights-version-001d)

- [Release Highlights: Version 0.0.1c](#release-highlights-version-001c)

- [Release Highlights: Version 0.0.1b](#release-highlights-version-001b)

- [1. Overview & Lore of Tydeshire](#1-overview--lore-of-tydeshire)

  - [The Great Unification & The Five Crowns](#the-great-unification--the-five-crowns)

  - [The Fracture & The Long Road](#the-fracture--the-long-road)

  - [Lore Fragments Codex (I–X)](#lore-fragments-codex-ix)

  - [Characters & Royal Court](#characters--royal-court)

- [2. Campaign Progression & World Design](#2-campaign-progression--world-design)

  - [Act I: The Ruined Castle](#act-i-the-ruined-castle)

  - [Act II: The Crossroads of the Damned](#act-ii-the-crossroads-of-the-damned)

  - [Realm Transition Screen](#realm-transition-screen)

  - [Act III: Fortified Realm of Zyphden](#act-iii-fortified-realm-of-zyphden)

  - [Hearthside Tavern](#hearthside-tavern)

  - [Expanded Zyphden Royal Keep](#expanded-zyphden-royal-keep)

  - [Act IV: Subterranean Quarry Depths & Primordial Altar](#act-iv-subterranean-quarry-depths--primordial-altar)

  - [Combat Arena & Endless Wave Defense](#combat-arena--endless-wave-defense)

- [3. Combat Systems & Mathematical Formulas](#3-combat-systems--mathematical-formulas)

  - [Defense & Armor Penetration Formula](#defense--armor-penetration-formula)

  - [Will Stat & Stagger Duration Formula](#will-stat--stagger-duration-formula)

  - [Anti-Stunlock Buffer](#anti-stunlock-buffer)

  - [Invulnerability Frames (i-frames)](#invulnerability-frames-i-frames)

  - [Parry Guard, Counter Buff & Projectile Reflection](#parry-guard-counter-buff--projectile-reflection)

  - [Consumable Health Elixirs & Out-of-Combat Regeneration](#consumable-health-elixirs--out-of-combat-regeneration)

- [4. Controls & Interface Reference](#4-controls--interface-reference)

  - [Keybindings Guide](#keybindings-guide)

  - [Modular Visual Action Bar](#modular-visual-action-bar)

  - [Attributes & Posture Status Indicators](#attributes--posture-status-indicators)

  - [In-Game Lore Journal, Equipment Paper Doll & Realm Map](#in-game-lore-journal-equipment-paper-doll--realm-map)

- [5. Weapon Arsenal, Armor & Totems](#5-weapon-arsenal-armor--totems)

  - [Arsenal Summary Table](#arsenal-summary-table)

  - [Detailed Weapon Stances](#detailed-weapon-stances)

  - [Sacred Totems Compendium](#sacred-totems-compendium)

- [6. Bestiary & Boss Encounters](#6-bestiary--boss-encounters)

  - [Hostiles Compendium](#hostiles-compendium)

  - [Boss Encounters & AI Cadence](#boss-encounters--ai-cadence)

- [7. Technical Architecture & Engine Pipeline](#7-technical-architecture--engine-pipeline)

  - [Directory Hierarchy](#directory-hierarchy)

  - [State Coordinator Pattern](#state-coordinator-pattern)

  - [Collision Geometry & Hit Registration](#collision-geometry--hit-registration)

  - [Procedural Acoustic Audio Synthesis](#procedural-acoustic-audio-synthesis)

  - [Custom Asset Drop Pipeline](#custom-asset-drop-pipeline)

- [8. Building & Setup Guide](#8-building--setup-guide)

  - [Automated MinGW-w64 Batch Script](#automated-mingw-w64-batch-script)

  - [CMake & Ninja Build](#cmake--ninja-build)

  - [Precompiled Executables](#precompiled-executables)

- [9. Roadmap & Version 0.0.1d Technical Plans](#9-roadmap--version-001d-technical-plans)

  - [Act V: Northern Frontier Borders](#act-v-northern-frontier-borders)

  - [Environmental Weather Physics](#environmental-weather-physics)

  - [Synthesized Environmental Ambience & Audio Sliders](#synthesized-environmental-ambience--audio-sliders)



---



## Release Highlights: Version 0.0.1d

A comprehensive breakdown of all systems, gameplay mechanics, visual overhauls, and architectural enhancements introduced in Version 0.0.1d:

* **Modular InputRouter & Centralized Hotkey Architecture**:
  * Decoupled key event polling from rendering passes (Tutorial.cpp, CrossroadsHandler.cpp, ZyphdenHandler.cpp), resolving dropped [O] objectives toggles caused by Raylib single-frame buffer consumption in render loops.
  * Centralized all modal navigation and hotkey dispatching in Game::Update, providing smooth cross-modal switching ([M] for Map, [I] / [TAB] for Gear, [C] for Controls, [ESC] for exit) without modal stack lockouts.

* **Royal Halberd Rebalance & 3-Stage Heavy Attack Combo**:
  * Rebalanced weapon damage: adjusted base light damage from 24.0f to 16.0f and base heavy attack from 88.0f to 28.0f (+1: 20/34, +2: 24/40; defense pierce bonus +20% on tier 2).
  * Implemented 3-stage heavy combo progression:
    * **Stage 1 — Long Lunge with i-frames**: Forward thrust surge (elocity += forwardDir * 320.0f) with active evasion invulnerability (iFrameTimer = 0.22f), linear LineBox collision, and motion blur trails.
    * **Stage 2 — Wide Sweeping Cleave**: Wide 220-degree horizontal polearm cleave covering front and flanks with heavy poise stagger.
    * **Stage 3 — Triple Consecutive Rapid Stabs**: Flurry of 3 rapid linear thrusts (0.09s interval), each dealing exactly 30% of base weapon damage with multi-hit spark particles.

* **Tactical Gear & Inventory Screen Overhaul (Image 1 Reference)**:
  * Top navigation bar with horizontal tabs (QUESTS | GEAR | INVENTORY | ABILITIES) featuring active glowing coral underline.
  * Attributes strip displaying real-time currency (Shingles & Ore Scraps), Health (HP), Attack (ATK), Defense (DEF), Speed (SPD), and Capacity.
  * Symmetrical 3-column tactical layout:
    * Left Column (Armaments): MAIN card with glowing red/coral border, vector wireframe weapon illustration, DMG/reach stats, and [1-4] quick cycle; SIDEARM, MELEE, SPECIAL (Totem Relic), and GRENADES.
    * Center Column (Hero Showcase): Animated paper doll (AssetManager::DrawCharacterDoll), glowing pedestal circle, ambient sparks, and bottom-left progression badge (23 JUST_BUCK_IT | SOLSDEN VANGUARD).
    * Right Column (Tactical Apparel & Potions): 2x2 grid for HEAD, CHEST, BOOTS, LEGS, and an interactive POTION quickslot ([E]) with live count and cooldown indicator.
    * Floating dark glass inspection tooltips on slot hover.

* **Tactical Frosted Realm Map Screen Overhaul (Image 2 Reference)**:
  * Frosted dark teal glass canvas with subtle Cartesian isometric grid overlay and glowing corner brackets.
  * Header with decorative wing lines (—o— Game Stage —o—), 3 golden stars, and chronicle subtitle.
  * Five numbered diamond waypoints (<1>–<5>) representing Ruined Castle, Crossroads, Zyphden Keep, Quarry Mines, and Northern Marches.
  * Real-time cyan radar beacon pulsing on the active player stage, connected by luminous leylines.
  * Floating left stage inspection card with STAGE #X title, ability badges, threat rating, fragment count, and [DISCOVER MORE].
  * Hexagonal bottom-right call-to-action button ([ RESUME JOURNEY ]).

* **Royal Halberd Weapon Archetype**: Added the 5th martial weapon to the player's arsenal. Features extended linear piercing thrusts (line box collision) that impale multiple foes with innate armor penetration, and a 2-stage heavy cleave combining a wide 220-degree sweeping axe cleave with a high-velocity forward pole-vault thrust. Fully integrated into Blacksmith Varek's Forge craft recipes (10 Ore Scraps + 35 Silver Shingles) and upgrade paths (+1 Refined, +2 Vanguard Polearm).

* **Procedural Acoustic Ambience Soundscapes**: Real-time algorithmic audio generation synthesizing continuous background soundscapes without external audio files—howling gale winds across mountain ramparts, resonant sub-bass drops and cavern reverberations in the subterranean Quarry, and crackling hearth embers inside the Tavern and Forge.

* **In-Game Audio Mixer (`[C]` -> `AUDIO MIXER`)**: Interactive volume control sliders inside the settings modal providing real-time gain balancing for Master Gain, Combat Sound Effects (SFX), and Environmental Ambience, complete with interactive test triggers.

* **Decoupled Modal Architecture Polish**: Eliminated residual draw calls executed during update ticks inside `RuinedCastleHandler` for tutorial dialogues and boss defeat rematches, guaranteeing full decoupling and stable framerate pacing.

* **6-Slot Weapon Save Persistence**: Serialized the 6th weapon slot into `save_slot_1.json` through `save_slot_3.json` to persist unlocked states and upgrade tiers without data regressions.



---



## Release Highlights: Version 0.0.1c



A comprehensive breakdown of all systems, gameplay mechanics, visual overhauls, and architectural enhancements introduced in Version 0.0.1c:



* **Interactive Realm Map (`[M]`)**: Comprehensive medieval cartographic guild survey map featuring smooth pan and zoom navigation, recenter shortcuts (`[SPACE]`/`[R]`), live player coordinate tracking beacon with radar pulses, and 8 interactive Points of Interest categorized by Quests, Services, and Danger with filter toggles (`[0]`-`[3]`).

* **Gated Quarry Depths & `QUARRY_TRANSITION`**: Gated eastern archway in Zyphden Courtyard that unlocks upon defeating Knight Lenon in the martial spar. Interacting transitions via a dedicated lore screen into the subterranean `MapEnvironment::QUARRY_DEPTHS` zone with slate flagstone paths and minecart tracks.

* **Ore Golem Patrols & Crafting Scraps**: Added 4 basalt Ore Golem spawners across the Quarry Mines with 7.0-second respawn timers. Defeated golems drop Ore Scraps used alongside Silver Shingles at Blacksmith Varek's Forge to craft the Dual Daggers and War Mace.

* **Primordial Altar & Secret Boss "Elemental Golem"**: Located deep within the Quarry Mines, the Primordial Altar requires collecting all 10 Lore Fragments (`frag_01` through `frag_10`). Assembling all fragments summons the titan secret boss (**Elemental Golem**, 850 HP, 55 Def, 90 Will) wielding 4-phase elemental quake, magma boulder, frost tremor, and storm fissure attacks.

* **Elemental Core Totem**: Defeating the Elemental Golem pacifies the altar, yields abundant Ore Scraps and Shingles, and permanently unlocks the `Elemental Core Totem` (+25 flat Defense, -15% movement speed, and auto-fires an `ELEMENTAL_BOLT` projectile on light attacks with a 5.0s cooldown).

* **Decoupled Screen-Space Modal Architecture**: Eliminated all `Draw*` calls from `Game::Update(float dt)`, resolving the freeze when opening the journal, map, or tab menu. Restructured `Game::RenderScene()` so all gameplay realms fall through to a single, centralized screen-space modal overlay executed safely between `BeginDrawing()` and `EndDrawing()`.

* **Dynamic Fullscreen Button Hit-Tests**: Replaced hardcoded dimensions with dynamic `GetScreenWidth()` and `GetScreenHeight()` calculations across title screen buttons, HUD top-right actions, and modals, ensuring pixel-perfect clicks across windowed, borderless, and 4K fullscreen modes.

* **Save File Quest Track Persistence**: Serialized `zyphdenQuestPhase` into save slots and reorganized `Game::LoadActiveGame` to restore inventory, equipment, and fragments before state transitions, preventing storyline regressions.

* **Lore Fragment VIII Relocation**: Moved Fragment VIII (*The Sovereign's Burden*) from inside the solid castle keep wall to `{ 1390.0f, 540.0f }` in the open Courtyard Sparring Training Grounds beside weapon racks, and updated its journal recovery clue.

* **Typography & Dialogue Polish**: Switched to crisp Windows `Georgia.ttf` rendering with bilinear filtering and proportional character spacing, and paginated Corrupted Zil's post-defeat monologue across 3 interactive pages with navigation hints.



---



## Release Highlights: Version 0.0.1b



A comprehensive breakdown of all systems, gameplay mechanics, visual overhauls, and architectural enhancements introduced in Version 0.0.1b:



* **Medieval Typography Engine**: Unified custom `FontManager` with custom glyph mappings, special character sanitization, and graceful ASCII fallbacks across all UI states, HUD elements, and dialogues.

* **Multi-Slot Persistence**: Independent 3-slot save system (`save_slot_1.json`, `save_slot_2.json`, `save_slot_3.json`) with automated background state saving and Title Screen slot selection with chapter metadata and timestamps.

* **Silver Shingles Economy & Physical Loot**: Physical ground coin drops with arc and bounce trajectory physics, vacuum magnet attraction toward the player, roaming overworld enemy respawns, and persistent HUD currency counters.

* **Blacksmith Forge & Tavern**: Introduced Blacksmith Varek with weapon upgrade tiers (Base, +1 Refined, +2 Masterwork, Consecrated Relic) and Tavernkeep Eldon offering consumable provisions, housed in dedicated interior environments.

* **Phlegmatic Composure & Four Humors**: Composure gauge (0-100), passive composure tiers (Agitated, Steady, Resolute, Phlegmatic), Phlegmatic Trance active invulnerability aura (`[SPACE]` / `[F]`), and humoral alignments (Sanguine, Choleric, Melancholic, Phlegmatic) with Quenching and Shattering mechanics.

* **Kinetic Combat Juice**: Hit-stop micro-pauses (0.04s - 0.06s) on heavy impact strikes and parries, paired with directional camera recoil impulses and trauma decay.

* **Animated Grimoire Lore Journal**: Replaced flat modals with a 3D animated dual-page medieval book featuring realistic page curl and shadow casting, illuminated drop caps, runic seal plaques, and explicit location discovery clues for all 10 fragments across Tydeshire.

* **Zyphden Gate Transition Fix & Artwork**: Resolved input coordinate mismatches during resolution scaling, enabled multi-key confirmation (`SPACE`, `ENTER`, `E`, and mouse click), and integrated custom 16:9 illustration of limestone ramparts at `assets/zyphden_transition.png`.

* **World Progression & Dialogue Polish**: Centered King Kreptu upon his royal dais with quest gating, conversational duel restarts with Knight Lenon after defeat, universal proximity-based prompt decluttering, and permanent suppression of tutorial enemy respawns following Zil's defeat.



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



> [!NOTE]

> **The Five Crowns Accord**: Each kingdom was chartered with a specific duty to preserve the continental leylines. Together, their five primordial seals held back the ancient void corruption lurking beneath Tydeshire.



### The Fracture & The Long Road

The Pax Charlz endured for three decades. But peace in Tydeshire was only ever borrowed. King Charlz succumbed to an unknown creeping madness, dissolving royal assemblies and vanishing into the northern frontiers without an heir. With the High Throne vacant, fear turned brother against brother. Border seals were fractured, and primeval monstrosities crawled forth from the mire.



The player begins their pilgrimage amidst the cold ashes of the southern sanctuary, armed with little more than instinct, bound on a long trek north through the corrupted **Crossroads** into the fortified gates of **Zyphden**.



### Lore Fragments Codex (I–X)

The game world contains ten discoverable historical fragments documented inside the in-game Journal (`[I]` or `[TAB]`):



| Fragment ID | Title | In-Game Location | Historical Significance |

| :--- | :--- | :--- | :--- |

| **`frag_01`** | **The Great Unification** | Awakening Courtyard Stele | Chronicles King Charlz crushing warring factions and chartering the Five Crowns. |

| **`frag_02`** | **The Shadow over Zyphden** | Eastern Ruins Chest & Noticeboard | Documents the first rumors of the Lord Liege's madness and envoy disappearances. |

| **`frag_03`** | **Zil's Holy Oath** | High Altar (Zil Remnant Duel) | The solemn vow of Sun-Crusader Zil as he rode south to anchor the high altars. |

| **`frag_04`** | **The Five Broken Seals** | Northwest Library Reliquary | Details the five elemental warding seals tethered directly to the High Crown. |

| **`frag_05`** | **The Crossroads Watch** | Crossroads Ancient Shrine | The tragic massacre of the southern border patrol when the highway went dark. |

| **`frag_06`** | **Rite of the Gale Winds** | Zyphden Wind Lectern | Architectural folios describing the resonance leylines powering Zyphden's ramparts. |

| **`frag_07`** | **Lenon's Field Log** | Courtyard Gates Lectern | Vanguard Captain Lenon's personal reflections on his missing sworn brother Zil. |

| **`frag_08`** | **The Sovereign's Burden** | Courtyard Sparring Training Grounds | King Kreptu's secret correspondence regarding dwindling grain stores and war. |

| **`frag_09`** | **Inscription of Ancient Pillars** | Inscribed Runic Monoliths | Ancient First Warden runes teaching how inner Will hardens posture against stagger. |

| **`frag_10`** | **The Royal Armory Archives** | Royal Keep Armory Chest | Tactical treaties detailing weapon armor penetration, poise breaks, and ripostes. |



> [!TIP]

> **Unread Notification Badges**: When a new fragment is uncovered in the world, the HUD Lore button and Journal tab display an amber alert badge (`(!)`). Open the codex to read the uncovered lore at any time.



> [!IMPORTANT]

> **REMINDER — Lore Journal Navigation**: You can inspect newly gathered fragments at any time during gameplay by pressing `[I]` or `[TAB]`. Reading lore steles grants immediate narrative context and sheds light on enemy weaknesses across Tydeshire's five realms.



> [!TIP]

> **Dialogue History Review**: Missed a line of dialogue? Use the `[A]` and `[D]` keys to flip pages backward and forward in any active dialogue window before pressing `[E]` or `[ESC]` to conclude.



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

The player awakes unarmed in an overgrown cathedral courtyard:

1. Inspect the central plinth to claim the **Crusader's Broadsword**.

2. Master movement (`[WASD]`), directional dashing (`[Q]`), parrying (`[F]`), and light/heavy sword slashes.

3. Slay the guardian Ruin Sentinel to dissolve the magical **Sanctuary Ward** sealing the high altar.

4. Duel **Zil's Remnant** at the high dais:

   - Defeating him awards the sacred **Truth Seeker (Partial)** and **Fragment III**.

   - If defeated, the player is granted an honorable rematch modal or can accept his mercy to receive **Zil's Token**.

5. Pass through the northern gateway to enter the Crossroads.



> [!IMPORTANT]

> **REMINDER — Sanctuary Ward**: You cannot approach the high altar until the initial Ruin Sentinel is defeated. Slaying the sentinel unseals the stone staircase leading to Zil's altar.



> [!TIP]

> **Zil's Remnant Mercy Option**: If you find Zil's Remnant too challenging on your first pilgrimage, accepting his mercy grants you **Zil's Token**, permitting safe passage through the northern gateway into the Crossroads without losing any story progression.



### Act II: The Crossroads of the Damned

A ruined wilderness connecting the southern ruins to the northern fortress:

* High-threat monster ambushes (Cultist Slashers, Deathseeker Archers, Void Pyromancers, and Shadow Chargers).

* Ancient shrine holding the **Totem of Sanity** (+15% Stamina Regen, +10 Will).

* Highway Inscribed Monolith (`frag_09`) and Crossroads Supply Chest (+2 Health Potions).

* Northern gateway leading into the Zyphden transition portal.



> [!TIP]

> **Telegraph Clarity**: Cultist Slashers and Chargers display directional red cones and linear paths. Deathseeker Archers show laser aim lines. Void Pyromancers project 3-way trajectory lasers with flaring ground landing tinges—step aside with `[Q]` or deflect projectiles back with `[F]`!



> [!IMPORTANT]

> **REMINDER — Totem of Sanity**: Do not miss the secluded shrine in the eastern clearing of the Crossroads. Claiming the Totem of Sanity grants a permanent **+15% Stamina Regeneration** and **+10 Will** bonus when equipped in your Journal inventory!



### Realm Transition Screen

An atmospheric realm-entry screen bridging the wilderness to Zyphden:

* Elegant dark stone presentation card with ambient dust particles and corner brackets.

* Automatic asset drop slot (`assets/zyphden_transition.png`): If present, displays custom 16:9 illustration; otherwise renders an artistic blueprint placeholder with dimension guides.

* Press `[SPACE]`, `[ENTER]`, or click the on-screen button to step across the threshold into Zyphden.



> [!TIP]

> **Custom Transition Illustration**: The transition screen natively checks for `assets/zyphden_transition.png`. Drop any 16:9 digital illustration or concept art into that location, and the game will automatically display it on your next journey across the realm threshold!



### Act III: Fortified Realm of Zyphden

The capital city of the gale winds:

* Cobblestone avenues, residential manors, market squares, and royal gardens.

* Town noticeboard and three lore historians (Cedric, Varek, Maeve).

* Three environmental lore items (`frag_06`, `frag_07`, `frag_08`).



### Blacksmith Varek's Forge

Located in the northern artisan sector of Zyphden:

* **Interactive Forge Anvil**: Press `[E]` near the forge anvil to open the weapon upgrading interface.

* **Tier Progression**: Spend earned **Silver Shingles** to upgrade discovered weapons:

  * **Refined (+1)**: Costs 100 Shingles. Grants +20% base damage and +15% poise damage.

  * **Masterwork (+2)**: Costs 250 Shingles. Unlocks critical hit enhancements and increased attack reach.

  * **Consecrated Relic**: High defense piercing and radiant projectile arcs.



### Hearthside Tavern & Eldon's Provisions

Cozy indoor retreat with a roaring fireplace, timber bar, and warm acoustics:

* Meet **Tavernkeep Eldon** behind the counter to initiate the royal questline.

* **Provisions Counter**: Purchase restorative **Health Potions** for **35 Silver Shingles** each to restock your flasks before royal trials.

* Triggers scripted entrance of **Knight Lenon** entering through the tavern doorway to inspect your blade or Zil's token.



> [!IMPORTANT]

> **REMINDER — Tavern Progress Trigger**: The inner courtyard of the Royal Keep is guarded by sovereign sentinels. Speak with Tavernkeep Eldon in the Hearthside Tavern first to prompt Knight Lenon's arrival and receive your formal audience summons.



### Multi-Slot Save Architecture

* **3-Slot Save Manager (`SaveManager`)**: Stores complete campaign progression across `save_slot_1.dat`, `save_slot_2.dat`, and `save_slot_3.dat`.

* **State Persistence**: Records active realm/chapter, player coordinates, remaining HP, Composure, equipped weapon/armor/totem, inventory weapon upgrades, unlocked lore fragments, Silver Shingles purse, health potion stock, and boss defeat milestones.

* **Title Record Modal**: Allows players to select active save slots, inspect saved playtime metrics, start fresh adventures, or safely delete deprecated files with verification prompts.

* **Auto-Save Frequency**: Saves automatically upon realm transitions, boss victories, monolith readings, vendor upgrades, and returning to the Title Screen (`~`).



### Expanded Zyphden Royal Keep

Grand stone hall expanded to **1200x1100** bounds:

* Stepped royal dais with King Kreptu seated upon his golden throne at `{ 1000, 275 }`.

* 940px crimson runner carpet leading from the southern entry portal directly to the throne, marked with the gold Sovereign Seal medallion.

* Eight solid marble colonnade pillars, Western Royal Archives with bookshelves, and Eastern Armory with weapon displays.

* Five interactive court NPCs (Malakor, Donald, Garrick, Thaddeus, Genevieve).

* Inscribed Monolith (`frag_09`), Armory Chest (+2 Potions, `frag_10`), and Provisions Chest (+1 Potion).



### Act IV: Subterranean Quarry Depths & Primordial Altar

Gated behind defeating Knight Lenon in the courtyard martial spar:

* **The Eastern Archway & Transition Screen**: Interacting with the eastern rampart portal (`[E]`) triggers `GameState::QUARRY_TRANSITION`, presenting illustrated lore exposition of Zyphden's ancient mineral shafts.

* **Quarry Mines Environment**: Underground cavern with carved basalt walls, glinting crystalline mineral seams, slate flagstone walkways, and ancient minecart rail circuits.

* **Ore Golem Patrols & 7-Second Spawners**: Four basalt Ore Golems patrol the shafts. Upon defeat, they drop valuable Ore Scraps and automatically reconstitute every 7.0 seconds.

* **The Primordial Altar Dais**: Located at `{ 1000, 320 }`, surrounded by 4 monolithic elemental pillars. Gathering all 10 Lore Fragments awakens the secret titan boss: **Elemental Golem (Primordial Titan)**. Conquering the titan bestows the **Elemental Core Totem**.



### Combat Arena & Endless Wave Defense

Unlocked permanently upon completing the Chapter 1 storyline:

* Accessible directly from the Title Screen or the Lobby portal.

* 5 escalating combat waves featuring mixed vanguard formations, dual-brute shockwaves, and the climactic boss encounter against **The Grand Inquisitor**.

* Adhesive Bandages (+30 HP) spawn periodically across the arena floor for sustained survival.



> [!TIP]

> **Arena Medical Pickups**: When facing overwhelming enemy waves in the Arena, watch for green glowing Adhesive Bandages on the floor. Each pickup restores **+30 HP** immediately upon touch.



> [!IMPORTANT]

> **REMINDER — Arena Checkpoints**: Completing all 5 waves of the Royal Combat Arena crowns you the Champion of Zyphden and permanently records your victory in the game state.



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



> [!IMPORTANT]

> **REMINDER — Armor vs. Pierce**: Enemies with heavy plate (such as Iron Brutes) heavily mitigate light sword strikes. Switch to the War Mace or land timed parry ripostes to pierce their defenses.



### Will Stat & Stagger Duration Formula

When an entity is parried or struck by devastating heavy blows (e.g. Iron Brute slams, Charger thrusts, boss smites), they enter a **Staggered** state where actions are disabled:



```text

Stagger Duration = Base Stagger * 100 / (100 + Will * 1.25)

```



#### Player Will Scaling

* **Unarmored (Starting)**: 20 Will $\rightarrow$ **0.80s** stagger.

* **Crusader Plate**: 65 Will $\rightarrow$ **0.55s** stagger.



#### Enemy Will Scaling Upon Parry

* **Cultist Slasher** (10 Will) $\rightarrow$ **1.95s** stagger.

* **Deathseeker Archer** (15 Will) $\rightarrow$ **1.85s** stagger.

* **Void Pyromancer** (20 Will) $\rightarrow$ **1.76s** stagger.

* **Shadow Charger** (30 Will) $\rightarrow$ **1.60s** stagger.

* **Iron Brute** (50 Will) $\rightarrow$ **1.35s** stagger.

* **The Grand Inquisitor** (60 Will) $\rightarrow$ **1.25s** stagger.

* **Zil's Remnant** (65 Will) $\rightarrow$ **1.21s** stagger.

* **Knight Lenon** (70 Will) $\rightarrow$ **1.17s** stagger.



> [!IMPORTANT]

> **REMINDER — Will Stat Mechanics**: Will does not reduce incoming numeric damage—it accelerates your recovery from physical poise breaks. Higher Will lets you recover and raise your guard before enemies can chain attacks.



> [!TIP]

> **Break Stagger with Poise Buffs**: Equipping the Crusader Plate reduces your stagger duration from 0.80s down to 0.55s, allowing you to raise your shield before an enemy can land a follow-up attack.



### Anti-Stunlock Buffer

To ensure fair and responsive combat, recovering from a stagger immediately grants the player **1.8 seconds of Stagger Immunity**. During this grace window, heavy enemy blows will still deal damage but cannot chain-stagger the player.



> [!TIP]

> **Utilizing the Immunity Buffer**: When you break out of a stagger, you have a 1.8-second window where heavy attacks cannot knock you down again. Use this window to dash away (`[Q]`) or immediately counter-strike!



### Invulnerability Frames (i-frames)

Activating **Dash (`[Q]`)** triggers a rapid 8-directional burst with **0.25 seconds of invulnerability frames**. During this active evasion window:

* Melee swings and shockwaves pass through the player harmlessly.

* Projectiles are safely phased through.

* Spawns an amber-white `"EVADED"` combat badge above the player's head.



> [!IMPORTANT]

> **REMINDER — Dash Recovery Window**: While Dash grants 0.25s of complete invulnerability (`EVADED`), it has a weapon-dependent recovery cooldown (0.50s - 1.25s). Timing your dashes deliberately is far more effective than spamming.



### Parry Guard, Counter Buff & Projectile Reflection

Pressing **Parry (`[F]`)** activates a golden cyan-tinted shield barrier for **0.24 seconds**:

* **Melee Deflection**: Parrying a melee attack perfectly negates all damage, inflicts **STAGGERED!** on the attacker, and grants the player a **+50% (1.5x) Counter-Attack Damage Multiplier** for 2.4 seconds.

* **Projectile Reflection**: Parrying an incoming enemy arrow or spell projectile deflects it back toward your mouse reticle with double speed and amplified damage.



> [!TIP]

> **Mastering Projectile Reflection**: You do not need to aim at the shooter to reflect. The projectile will fly directly toward wherever your **mouse cursor** is pointing at the instant of parry!



> [!IMPORTANT]

> **REMINDER — Parry Guard Timing**: The active parry window lasts for exactly 0.24 seconds. Activate it just as the enemy's attack swing begins forward motion for a guaranteed stagger and the +50% counter-damage boost.



### Consumable Health Elixirs & Out-of-Combat Regeneration

* **Health Potions (`[1]` or `[H]`)**: Drinking an elixir restores **+45 HP** immediately and initiates a **7.0-second cooldown timer**.

* **Out-of-Combat Regeneration**: Staying disengaged from combat for **4.0+ seconds** activates passive health recovery at **+8.0 HP/s** up to maximum capacity.



> [!IMPORTANT]

> **REMINDER — Potion Cooldown**: Health potions carry an intentional 7.0-second cooldown to prevent flask-spamming during intense boss duels. Monitor your visual action bar icon to track the remaining recovery time.



> [!TIP]

> **Out-of-Combat Health Recovery**: If you run out of potions, disengage from enemies and avoid taking damage for 4.0 seconds. Your vitality will automatically regenerate at +8.0 HP per second until fully healed!



### Phlegmatic Composure System & Stoic Poise

The **Phlegmatic Composure** system governs the player's mental resilience, deliberate focus, and unflinching poise under intense combat stress.



#### Composure Mechanics

* **Gauge Bounds**: `0.0f` (Discomposed) to `100.0f` (Max Focus / Trance Ready).

* **Gaining Composure**:

  * **Timed Parry (`[F]`)**: **+25.0 Composure** on successful deflection.

  * **Evasive Dash (`[Q]`)**: **+15.0 Composure** when phasing through enemy hitboxes during i-frames.

  * **Quenching Choleric Frenzy**: **+30.0 Composure** bonus when deflecting a Choleric hostile.

  * **Patient Stance**: **+8.0 Composure/s** for standing unhit within 220px of a telegraphing enemy without spamming attacks.

* **Losing Composure**:

  * **Unmitigated Heavy Hits**: **-25.0 Composure** on taking damage from heavy enemy attacks.

  * **Exhausted Swings**: **-15.0 Composure** when attempting heavy attacks without sufficient stamina.

  * **Natural Decay**: Slowly decays at **-1.5 Composure/s** when disengaged from combat.



#### Composure Poise Tiers

* **Discomposed (< 30.0)**:

  * Indicated by a pulsating red alert badge (`POISE: LOW [DISCOMPOSED]`).

  * **+15% Stagger Duration Penalty**: Vulnerable to extended enemy stagger chains.

* **Composed (30.0 – 79.9)**:

  * Baseline state of mental equilibrium (`POISE: XX [COMPOSED]`).

  * Normal recovery and standard stamina regeneration.

* **Stoic Poise (80.0 – 100.0)**:

  * Indicated by a radiant gold badge (`POISE: MAX [STOIC POISE]`).

  * **Hyper-Armor**: Complete immunity to stagger and knockback during heavy attack windups.

  * **Hemostasis**: Complete immunity to Sanguine arterial bleed damage.



#### Active Ability — Phlegmatic Trance (`[SPACE]`)

When Composure reaches maximum capacity (**100.0**), the player can press `[SPACE]` to enter the mystical **Phlegmatic Trance** for **5.0 seconds**:

* **Complete Invulnerability**: Complete damage immunity (`TRANCE INVULN!`).

* **Zero Stamina Drain**: All light, heavy, and dash actions consume 0 stamina.

* **Automatic Reflection Sphere**: Automatically reflects all enemy projectiles within a **120px spherical aura** without requiring manual parry inputs.



---



### Medieval Four Humors Alignment

Enemies across Tydeshire are aligned with the ancient Medieval Four Humors doctrine, exhibiting distinct behavioral temperaments, visual auras, and tactical vulnerabilities:



| Humor Type | Element & Color | Combat Temperament | Tactical Vulnerability & Interaction |

| :--- | :--- | :--- | :--- |

| **Choleric** | Yellow Bile (Amber Fire `[255, 140, 40]`) | Relentless aggression, furious charge velocity, and shortened windups. | **Quenching**: Deflecting a Choleric strike with a timed parry extinguishes their fury, applying **QUENCHED!** (-30% movement speed for 5.0s, **2.0x stagger duration**, and grants **+30 Composure** to the player). |

| **Melancholic** | Black Bile (Earth/Void `[160, 60, 255]`) | Hardened defense carapace granting **+50% base armor** and stoic resilience. | **Shattering**: Striking with a War Mace or 2-stage heavy cleave shatters their carapace, applying **SHELL SHATTERED!** (**-35% defense reduction** for 4.0s). |

| **Sanguine** | Blood (Arterial Crimson `[255, 60, 75]`) | Rapid flurry strikes and piercing arrows that inflict arterial hemorrhage. | **Hemorrhage**: Unmitigated strikes inflict **Bleed** (10 damage over 3.0s). Reaching **Stoic Poise (80+ Composure)** provides complete immunity against bleed procs. |

| **Phlegmatic** | Cold Water (Azure Cyan `[80, 220, 255]`) | Deliberate defense, steady positioning, and calculated guard counters. | **Patience & Poise**: Vulnerable to guard breaks, heavy tremors, and timed counter-ripostes. |



---



### Combat Juice: Hit-Stop & Directional Camera Shake

* **Hit-Stop Micro-Pause**: Delivers a visceral 2-3 frame micro-pause (**0.04s - 0.06s freeze**) upon parry deflections, heavy cleaves, projectile reflections, and armor shell breaks. During hit-stop, entity simulation pauses while camera shake and particle effects continue, creating impactful, crunchy combat feedback.

* **Directional Camera Shake**: Smooth directional impulse camera shake reacting directly to the trajectory of strikes, tremors, and deflections, blending impulse vectors with decay jitter for realistic recoil physics.



---



## 4. Controls & Interface Reference



### Keybindings Guide



| Key / Input | Action Name | Gameplay Function |

| :--- | :--- | :--- |

| `W, A, S, D` / `Arrows` | Movement | 8-directional traversal with solid obstacle collision. |

| `Mouse Cursor` | Aiming | Reticle aiming for sword thrusts, dashes, and parry reflections. |

| `Left Mouse Button` | Light Attack | Swift slash/thrust with low stamina cost and quick recovery cadence. |

| `Right Mouse Button` | Heavy Cleave | Signature multi-stage strike, lunge, or shockwave. |

| `Q Key` | Dash (Evade) | Fast directional burst with **0.25s i-frames** (`EVADED`). |

| `F Key` | Timed Parry | Active guard window. Staggers melee foes and reflects missiles. |

| `SPACE Key` | Phlegmatic Trance | Available at 100 Composure. **5.0s invulnerability, zero stamina cost, 120px projectile reflection sphere**. |

| `1 Key` or `H Key` | Health Potion | Consumes 1 elixir to heal **+45 HP** (**7.0s cooldown**). |

| `E Key` / `Enter` | Interact | Talk to NPCs, open chests, read monoliths, and transition doors. |

| `A Key` / `D Key` | Dialogue Page | Navigate between previous and next pages of active dialogue. |

| `ESC Key` | Close / Dismiss | Safely dismiss open dialogues, popups, and journal modals. |

| `O Key` | Toggle Objectives | Expand or collapse the pinned Chapter 1 quest tracker card. |

| `I Key` or `TAB` | Lore & Equipment | Open the in-game Journal, Equipment stats, and Totem slots. |

| `M Key` | Interactive Realm Map | Open the full parchment map with live player tracking and POI tooltips. |

| `F11` / `Alt + Enter` | Fullscreen Toggle | Switch between windowed and native crisp fullscreen display modes. |

| `C Key` | Controls Guide | Toggle the quick-reference keybindings overlay. |

| `~ Key` (Tilde / Grave) | Title Screen | Instantly return to the Title Screen from any state. |

| `R Key` | Respawn / Retry | Instantly restart an active trial or respawn after defeat. |

| `L Key` | Return to Lobby | Warp back to the Armory Sanctuary from lobby-connected states. |



> [!NOTE]

> **Safe UI Dismissal**: Pressing `[ESC]` will safely dismiss any active modal, popup, or dialogue window without pausing or exiting the game.



> [!TIP]

> **In-Game Controls Overlay**: In the heat of battle, you don't need to pause or exit—simply press `[C]` at any moment to summon a semi-transparent HUD overlay showing all keybindings and combat shortcuts.



> [!IMPORTANT]

> **REMINDER — Objectives Tracker**: Press `[O]` to expand or collapse the active Chapter 1 quest tracker. It tracks your current primary task, whether claiming your initial weapon or seeking audience with King Kreptu.



### Modular Visual Action Bar

Located at the bottom center of the HUD, the visual action bar provides real-time ability tracking:

* **Slot 1 (`[Q]`)**: Dash — Icon, active evasion pulse, and recovery sweep.

* **Slot 2 (`[LMB]`)**: Light Attack — Cadence recovery wipe.

* **Slot 3 (`[RMB]`)**: Heavy Attack — Multi-stage cooldown sweep with numeric timer (`2.1s`, `3.6s`).

* **Slot 4 (`[F]`)**: Parry — Radiant gold when ready, cyan flash on active guard, dark recovery wipe.

* **Slot 5 (`[SPACE]`)**: Phlegmatic Trance — Cyan focus medallion glowing when ready (`TRANCE READY [SPACE]`), countdown sweep when active (`5.0s`).

* **Slot 6 (`[1]`)**: Health Flask — Emerald theme, stock badge (`xN`), and **7.0s countdown timer**.



### Attributes & Posture Status Indicators

Displayed directly alongside the Player Health, Stamina, and Composure bars:

* **DEF: XX**: Total active defense rating including equipped armor.

* **WILL: XX**: Current willpower stat mitigating incoming stagger durations.

* **SHINGLES: XX**: Active purse of Silver Shingles for blacksmith forging and apothecary provisions.

* **POISE: XX [STATUS]**: Real-time Composure poise tier:

  * `[DISCOMPOSED]` (Red, < 30 Composure): +15% stagger duration vulnerability.

  * `[COMPOSED]` (Slate Blue, 30-79 Composure): Equilibrium.

  * `[STOIC POISE]` (Radiant Gold, 80-100 Composure): Hyper-Armor immunity to stagger and bleed during heavy attack windups.

* **Dynamic Posture Pill**: Displays real-time state: `STAGGERED!`, `POISE GUARD`, `I-FRAMES`, `+50% COUNTER`, or `COMBAT READY`.



### In-Game Lore Journal, Equipment Paper Doll & Realm Map

Opened via `[I]` or `[TAB]` (or `[M]` directly for the Realm Map):

* **Tab 1: Equipment & Arsenal**: Inspect weapons, weapon upgrade tiers (+1, +2, Consecrated), armor sets, damage mitigation statistics, and equip discovered sacred totems.

* **Tab 2: Lore Journal**: Browse 10 discovered historical fragments with unread badges (`(!)`).

* **Tab 3: Interactive Realm Map (`[M]`)**: Inspect the cartographic survey of Tydeshire, pan/zoom across terrain, filter POIs, and locate the player beacon.



---



## 5. Weapon Arsenal, Armor & Totems



### Arsenal Summary Table



| Weapon | Light Attack | Heavy Cleave | Dash CD | Special Properties & Passives |

| :--- | :--- | :--- | :--- | :--- |

| **Unarmed** | None | None | 0.75s | Starting state in cathedral awakening. |

| **Crusader's Broadsword** | 18 dmg (110° arc) | 70 dmg (2-Stage cleave) | 0.85s | Balanced reach. Heavy cleave consists of Stage 1 wide 160° sweep (42 dmg) followed by Stage 2 upward line/box shockwave (28 dmg). |

| **Truth Seeker (Partial)** | 26 / 24 dmg (2-Hit) | 78 dmg (Dual 360° + Beam) | 0.65s | Consecrated relic blade. Light attack chains a slash into an upward radiant slice. Heavy unleashes dual 360° sweeps (42 + 36 dmg) and fires a holy piercing beam (22 dmg, 10% pierce). |

| **Dual Daggers** | 11 dmg (**0 Stamina**) | 60 dmg (Lunge + Spin) | 0.50s | Zero stamina light attacks. Heavy executes a high-velocity piercing lunge (**22 dmg + i-frames**) followed by a 360° whirlwind (**38 dmg**). |

| **War Mace** | 28 dmg (Frontal Smack) | 96 dmg (Earthshaker) | 1.25s | Massive poise crusher. Heavy executes an earth-shattering overhead smash followed by a 360° ground tremor that **ignores 20% of enemy defense**. |



### Detailed Weapon Stances



#### Crusader's Broadsword

* **Stance Philosophy**: Standard military vanguard doctrine. Exceptional balance of defense and reach.

* **Tactical Advice**: Use the wide Stage 1 cleave to control crowds of slashers, then let Stage 2 catch fleeing archers.



#### Truth Seeker (Partial)

* **Stance Philosophy**: Holy sun-crusader relic. Emits sacred dawnfire that pierces corrupted flesh.

* **Tactical Advice**: The holy beam fired at the end of the heavy combo travels up to 400px. Line up multiple enemies to pierce through an entire column!



#### Dual Daggers

* **Stance Philosophy**: Fluid, predatory skirmishing. Relies on agility and breathless recovery.

* **Tactical Advice**: Light attacks cost **0 stamina**. Keep slicing continuously to apply pressure, then use the heavy lunge's built-in i-frames to phase through counter-attacks.



#### War Mace

* **Stance Philosophy**: Heavy blunt trauma designed to crush fortified armor plates.

* **Tactical Advice**: Heaviest weapon in the game. Stage 2 Earthshaker tremor ignores 20% of enemy defense. Ideal against Iron Brutes and Knight Lenon.



### Sacred Totems Compendium



Sacred Totems are ancient spiritual relics recovered across Tydeshire's ruins, altars, and boss encounters. Players can equip one active totem in the Journal / Equipment interface (`[I]` or `[TAB]` -> Equipment Tab) to augment their combat style:



| Totem Relic | Acquisition Source | Stat Multipliers & Buffs | Drawbacks & Nerfs | Visual Glow |

| :--- | :--- | :--- | :--- | :--- |

| **Totem of Sanity** | Hidden ruins along Crossroads path | **+15% Overall Damage, +15 Will** | None (Pure Relic) | Emerald Mint (`[100, 235, 180]`) |

| **Totem of Insanity** | Cathedral crypt reliquary | **+30% Overall Damage** | **-10% Armor Defense** | Arterial Crimson (`[255, 65, 80]`) |

| **Totem of Mist** | East Crossroads shroud shrine | **+20% Stamina Regen, +5% Cooldown Speed, +5 Will** | **-5% Overall Damage** | Vapor Azure (`[140, 210, 255]`) |

| **Crusader's Totem** | Defeating Knight Lenon in Royal Keep | **+30% Armor Defense, +20 Will** | **-10% Movement Speed** | Radiant Gold (`[255, 215, 80]`) |

| **Elemental Core Totem** | Defeating Elemental Golem (Quarry Depths) | **+25 Defense, Projectile on Light Attack (5.0s Cooldown)** | **-15% Movement Speed** | Molten Amber (`[255, 130, 45]`) |



> [!TIP]

> **Elemental Core Synergy**: The Elemental Core Totem fires a high-velocity elemental projectile bolt whenever you perform a light attack (with an internal 5.0-second cooldown). This allows melee fighters to weave ranged pressure between close-quarters blade swings!



> [!TIP]

> **Weapon Swapping in Lobby**: You can swap weapons freely between missions at the pedestals in the Lobby sanctuary. Try different armaments in the Arena to discover your preferred combat style!



> [!IMPORTANT]

> **REMINDER — Heavy Attack Commitment**: Heavy attacks (`[RMB]`) deliver massive burst damage but commit your character to the swing trajectory. Unleash heavy attacks when the target is reeling from a stagger or during recovery windows.



> [!TIP]

> **Dual Daggers Stamina Advantage**: Dual Daggers consume zero stamina for light slashes, leaving all your stamina pool free for rapid evasive dashes and heavy lunges.



---



## 6. Bestiary & Boss Encounters



### Hostiles Compendium



| Enemy Name | Type | Def | Will | Telegraph Indicator | Combat Behavior & Strategy |

| :--- | :--- | :--- | :--- | :--- | :--- |

| **Cultist Slasher** | Melee Vanguard | 8 | 10 | Amber/Red Cone (95°, 115px) | Swift forward slash. Sidestep or parry right before impact to stagger for 1.95s. |

| **Deathseeker Archer** | Ranged Kiting | 6 | 15 | Red Laser Aim Line (360px) | Retreats when approached; fires piercing bolts. Parry reflects bolt directly back. |

| **Void Pyromancer** | Occult Caster | 10 | 20 | 3-Way Laser & Glowing Landing Tinges | Casts tri-directional void orbs. Orbs can be reflected back with `[F]`. |

| **Shadow Charger** | Dash Melee | 12 | 30 | Linear Path (50px width, 240px) | High-speed linear thrust. Evade laterally with `[Q]` or parry head-on. |

| **Iron Brute** | Heavy Crusher | 32 | 50 | Expanding Red Circle (150px AoE) | High armor tank. Ground slam inflicts heavy stagger if unevaded. |

| **Ore Golem** | Melancholic Tank | 45 | 55 | Expanding Red Circle (135px AoE) | High-durability mineral chassis (220 HP, 45 Def). Smashes ground with heavy stone tremors. Best countered with War Mace or heavy 2-stage cleaves to shatter stone armor. Respawns after 7.0s and drops Ore Scraps for smithing. |



> [!TIP]

> **Countering Cultist Slashers**: Cultist Slashers always telegraph with an amber/red cone before swinging. A single well-timed parry (`[F]`) leaves them staggered for nearly 2 full seconds (1.95s), ample time for a full heavy combo.



> [!IMPORTANT]

> **REMINDER — Red Telegraphed Attacks**: Attacks displaying red ground telegraphs (cones, lines, circles) cannot be interrupted by light attacks. Either dodge out with `[Q]` or parry at the exact moment of impact.



### Boss Encounters & AI Cadence



#### Zil's Remnant (Shadow of the Sun Crusader)

* **Stats**: 520 HP, 28 Defense, 65 Will, 115 MoveSpeed.

* **Moveset**: Upward Radiant Box Slice, Dawnfire 360 Tremor Sweep, and Holy Beam piercing blast.

* **Special Phase**: Features a telegraphed follow-up cleave preceded by a **0.42s blade gleam telegraph** (140° arc, 165px range).

* **Strategy**: Do not get greedy during his 3-hit combo. Parry his initial slice, evade the 360 sweep with `[Q]`, and punish during his beam recovery.



#### Knight Lenon (Vanguard Captain Spar)

* **Stats**: 640 HP, 32 Defense, 70 Will, 125 MoveSpeed.

* **Moveset**: Rapid 0.48s lunge thrust, 0.55s whirlwind cleave, and 0.45s recovery cycle.

* **Reward**: Besting him awards the consecrated **Crusader Plate (+24 Def, +30 HP, 65 Will)** and unseals the Royal Combat Arena.

* **Strategy**: Lenon has 70 Will and recovers quickly from staggers. Break his posture with parries, unleash a quick 2-hit combo, and immediately prepare to dodge his follow-up lunge.



#### The Grand Inquisitor (Arena Boss)

* **Stats**: 500 HP, 26 Defense, 60 Will.

* **Moveset**: Rotates between 12-way Radial Star Trajectories, Linear Beam Shard, and Cataclysmic Smite AoE.

* **Strategy**: Stay mobile. Use dash i-frames to phase through radial stars, and parry his linear beam shards to reflect massive holy damage back into him.



#### Elemental Golem (Primordial Titan)

* **Stats**: 850 HP, 55 Defense (~35% damage reduction), 90 Will, 100 MoveSpeed.

* **Humor Alignment**: Melancholic [Earth / Primordial Magma].

* **Summoning Requirement**: Accessible in the Subterranean Quarry Depths after defeating Knight Lenon. Requires depositing all 10 recovered Lore Fragments upon the Primordial Altar.

* **Moveset & 4-Phase Combat Rotation**:

  * **Phase 1 (Ground Tremor Cleave)**: Charges a massive 190px circular shockwave (0.72s windup) dealing heavy blunt trauma.

  * **Phase 2 (Molten Shard Salvo)**: Conjures and unleashes high-velocity elemental projectiles aimed directly at the player.

  * **Phase 3 (Tri-Directional Volley)**: Hurls a tri-directional fan of elemental missiles across the quarry cavern.

  * **Phase 4 (Radial Nova Shockwave)**: Radiates an 8-directional elemental star burst, demanding precise timing on dash i-frames (`[Q]`) or parry reflections (`[F]`).

* **Reward**: Drops the **Elemental Core Totem**, 12–18 Ore Scraps, 150–250 Silver Shingles, and 3 Health Elixirs.

* **Tactical Strategy**: With 55 Defense and 90 Will, the Elemental Golem is nearly impervious to light spam. Utilize the **War Mace** or the Crusader Broadsword's 2-stage heavy cleave to trigger **SHELL SHATTERED!** (-35% defense). Keep mobile during projectile salvos, parry incoming shards to deflect damage back into the titan, and trigger **Phlegmatic Trance (`[SPACE]`)** at 100 Composure for 5.0 seconds of invulnerability and automatic projectile reflection.



> [!IMPORTANT]

> **REMINDER — Boss Stagger Windows**: Bosses possess high Will (60–90) and recover from staggers quickly. Never commit to more than a 2-hit sequence while a boss is reeling.



---



## 7. Technical Architecture & Engine Pipeline



### Directory Hierarchy

```text

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

├── documentation.md            # Complete game design & technical manual

└── CMakeLists.txt              # Cross-platform CMake configuration

```



### State Coordinator Pattern

State coordination is decoupled into discrete state handlers:

* `RuinedCastleHandler`: Manages Act I tutorial, sanctuary ward, and Zil encounter.

* `CrossroadsHandler`: Manages Act II wilderness encounters, ambushes, and gateway triggers.

* `ZyphdenHandler`: Manages Act III town logic, tavern sequence, expanded Royal Keep, and courtyard duel.

* `WaveManager`: Controls Arena trial waves, spawning algorithms, and medical pickup timers.



### Collision Geometry & Hit Registration

`CollisionSystem.cpp` performs precision geometric checks:

* Sector / Cone checks via dot products and angular offsets for weapon sweeps.

* Oriented bounding boxes for beam attacks and lunges.

* Circle-to-circle hit registration for projectile deflections and proximity pickups.



### Procedural Acoustic Audio Synthesis

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



> [!TIP]

> **Custom Illustration Auto-Detection**: Drop any custom 16:9 illustration into `assets/zyphden_transition.png`. You do not need to recompile the project—the engine detects the file on next launch and replaces the blueprint placeholder automatically!



> [!IMPORTANT]

> **REMINDER — Zero External Asset Dependencies**: By default, the engine generates all audio waveforms and vector character models algorithmically. The game can run fully standalone without requiring any external PNG or WAV files.



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



> [!TIP]

> **Instant Launch Without Compiling**: You can immediately test gameplay by executing `.\run.bat` or double-clicking `phlegmatic.exe` in the repository root. All Raylib 6.2 binaries and static assets are self-contained.



> [!IMPORTANT]

> **REMINDER — Clean Rebuilds**: If you modify core struct layouts or headers, run `build.bat` to trigger a clean GCC compilation. The script compiles all translation units with optimization level `-O2` in under 5 seconds.



---



## 9. Roadmap & Version 0.0.1d Technical Plans



### Act V: Northern Marches & Realm Border Crossings

* **Realm Border Gates**: Opening the northern mountain passes of Zyphden leading into the neighbouring nations of Azshden and Fleurshden.

* **Environmental Weather Physics**: Dynamic gale wind physics that deflect lightweight projectile paths, combined with alpine snow particle shaders and slippery frozen terrain.

* **New Hostile Factions**: Royal Vanguard Deserters wielding long-reach halberds, Frost Cultists casting ice barriers, and automated Siege Ballistas.



### Advanced Weapon Archetypes

* **Polearm / Halberd**: Extended piercing reach, sweeping pole-vault strikes, and anti-cavalry thrusts.

* **Flail & Greatshield**: Deflection-focused vanguard armament capable of absorbing heavy crushing blows without poise depletion.



### Synthesized Acoustic Ambience & Audio Mixer

* **Procedural Environmental Ambience**: Continuous synthesized background soundscapes (whistling wind drafts, subterranean cavern echoes, cathedral reverberations, and tavern fireplace crackle).

* **Audio Mixer Configuration**: Dedicated Master, Sound Effects, and Ambience volume control sliders accessible from the in-game settings modal.



### Bestiary Codex & Rematch Arena Trials

* **Bestiary Kill Counters & Lore Unlocks**: Track enemy encounters and unlock detailed lore excerpts and drop rate tables in the in-game Journal.

* **Sanctuary Rematch Crucible**: Challenge empowered variants of Zil's Remnant, Knight Lenon, and the Elemental Golem with custom trial modifiers and speedrun timers.

