# PHLEGMATIC - Complete Game Documentation & Technical Reference

<div align="center">

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Raylib](https://img.shields.io/badge/Engine-Raylib%20v6.2-white?style=for-the-badge&logo=raylib&logoColor=black&color=white)](https://www.raylib.com/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20(x64)-0078D6?style=for-the-badge&logo=windows&logoColor=white)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](LICENSE)

*The definitive technical reference manual, game system compendium, combat mathematics handbook, and narrative lore guide for **Phlegmatic**.*

---

</div>

## Table of Contents

- [Version History & Patch Notes (VERSIONS.md)](VERSIONS.md)
- [1. Overview & Lore of Tydeshire](#1-overview--lore-of-tydeshire)
  - [The Great Unification & The Five Crowns](#the-great-unification--the-five-crowns)
  - [The Fracture & The Long Road](#the-fracture--the-long-road)
  - [Lore Fragments Codex (I-XV)](#lore-fragments-codex-i-xv)
  - [Characters & Royal Court](#characters--royal-court)
- [2. Campaign Progression & World Design](#2-campaign-progression--world-design)
  - [Act I: The Ruined Castle](#act-i-the-ruined-castle)
  - [Act II: The Crossroads of the Damned](#act-ii-the-crossroads-of-the-damned)
  - [Realm Transition Screen](#realm-transition-screen)
  - [Act III: Fortified Realm of Zyphden](#act-iii-fortified-realm-of-zyphden)
  - [Blacksmith Varek's Forge](#blacksmith-vareks-forge)
  - [Hearthside Tavern, Provisions & Alchemy Workshop](#hearthside-tavern-provisions--alchemy-workshop)
  - [Multi-Slot Save Architecture & Storage Vault](#multi-slot-save-architecture--storage-vault)
  - [Expanded Zyphden Royal Keep](#expanded-zyphden-royal-keep)
  - [Act IV: Subterranean Quarry Depths & Primordial Altar](#act-iv-subterranean-quarry-depths--primordial-altar)
  - [Act V: Restless Mist Forest & Altar of Whispers](#act-v-restless-mist-forest--altar-of-whispers)
  - [Act VI: Northern Crags & The Permafrost Crypts](#act-vi-northern-crags--the-permafrost-crypts)
  - [Combat Arena & 25-Wave Humoral Gauntlet](#combat-arena--25-wave-humoral-gauntlet)
- [3. Combat Systems & Mathematical Formulas](#3-combat-systems--mathematical-formulas)
  - [Defense & Armor Penetration Formula](#defense--armor-penetration-formula)
  - [Will Stat & Stagger Duration Formula](#will-stat--stagger-duration-formula)
  - [Anti-Stunlock Buffer](#anti-stunlock-buffer)
  - [Invulnerability Frames (i-frames)](#invulnerability-frames-i-frames)
  - [Parry Guard, Counter Buff & Projectile Reflection](#parry-guard-counter-buff--projectile-reflection)
  - [Consumable Health Elixirs, Combat Tonics & Out-of-Combat Regeneration](#consumable-health-elixirs-combat-tonics--out-of-combat-regeneration)
  - [Phlegmatic Composure System & Stoic Poise](#phlegmatic-composure-system--stoic-poise)
  - [Medieval Four Humors Alignment](#medieval-four-humors-alignment)
  - [Combat Juice: Hit-Stop & Directional Camera Shake](#combat-juice-hit-stop--directional-camera-shake)
- [4. Controls & Interface Reference](#4-controls--interface-reference)
  - [Keybindings Guide](#keybindings-guide)
  - [Modular Visual Action Bar & 3-Slot Weapon Hotbar Dock](#modular-visual-action-bar--3-slot-weapon-hotbar-dock)
  - [Attributes & Posture Status Indicators](#attributes--posture-status-indicators)
  - [In-Game Equipment Matrix, Lore Journal, Bestiary & Realm Map](#in-game-equipment-matrix-lore-journal-bestiary--realm-map)
- [5. Weapon Arsenal, Armor & Totems](#5-weapon-arsenal-armor--totems)
  - [Arsenal Summary Table](#arsenal-summary-table)
  - [Detailed Weapon Stances](#detailed-weapon-stances)
  - [Armor Sets & Frostwarden Synergy](#armor-sets--frostwarden-synergy)
  - [Sacred Totems Compendium](#sacred-totems-compendium)
- [6. Bestiary & Boss Encounters](#6-bestiary--boss-encounters)
  - [Hostiles Compendium](#hostiles-compendium)
  - [Boss Encounters & AI Cadence](#boss-encounters--ai-cadence)
- [7. Technical Architecture & Engine Pipeline](#7-technical-architecture--engine-pipeline)
  - [Directory Hierarchy](#directory-hierarchy)
  - [State Coordinator Pattern](#state-coordinator-pattern)
  - [Collision Geometry & Hit Registration](#collision-geometry--hit-registration)
  - [GLSL 330 Multiplicative Day-Night Shader Pipeline](#glsl-330-multiplicative-day-night-shader-pipeline)
  - [Procedural Acoustic Audio Synthesis](#procedural-acoustic-audio-synthesis)
  - [Custom Asset Drop Pipeline](#custom-asset-drop-pipeline)
- [8. Building & Setup Guide](#8-building--setup-guide)
  - [Automated MinGW-w64 Batch Script](#automated-mingw-w64-batch-script)
  - [CMake & Ninja Build](#cmake--ninja-build)
  - [Precompiled Executables](#precompiled-executables)
- [9. Uncharted Realms Beyond Zyphden](#9-uncharted-realms-beyond-zyphden)

---

## Version History & Patch Notes

For complete release notes, balance revisions, and changelogs across versions (`v0.0.1b` through `v0.0.1k`), consult **[Version History & Patch Notes Compendium (VERSIONS.md)](VERSIONS.md)**.

### Latest Release: Version 0.0.1k

* **3-Slot Weapon Hotbar & Rebound Quick Belt**: Equip up to 3 weapons via Inventory `[SLOT 1/2/3]` and hot-swap mid-combat with `[1]`, `[2]`, `[3]` (persisted in JSON saves). Consumables moved to `[H]` (Health Potion), `[4]` (Stoneskin Tonic), `[5]` (Glacial Thaw Draught), and `[6]` (Berserker Brew).
* **`[TAB]` Inventory, `[B]` Bestiary & Modal Fixes**: Restored `[TAB]` as Inventory (`[I]` alias), repaired `[M]` Realm Map same-frame closing and Bestiary tab routing, added direct `[B]` Bestiary hotkey, and enabled top-right HUD clicks in northern/eastern realms.
* **Enlarged Vector-Icon HUD**: Upgraded bottom-center action slots (`58x62` / `64x68`) with procedural vector weapon, flask, shield, and armor skill icons alongside an enlarged bottom-right 3-Weapon Hotbar Dock (`64x66` / `70x72`).
* **GLSL 330 Multiplicative Day-Night Shader**: Replaced flat ambient washes with an offscreen `RenderTexture2D` multiplicative lightmap, inverse-square point lights, Purkinje composite tone-mapping, and full realm emitter registration.
* **Combat & Balance Reworks**: Removed the stagger-on-every-hit / Visceral Execution loop (replacing Phlegmatic Core per-hit stun with `3.0s` Chill); reworked **Twin Glacial Cleavers** (`4-stage Fury of Cleaves`), **Sun-Forged Claymore** (`2-stage Solar Cataclysm`), and **Storm Glaive** (`2-stage Tempest Cyclone`); revamped **Frostwarden** (`FROST NOVA`, `6.0s` `45 HP` `ICE WARD`, `30%` Cold Resist, `+20%` heavy poise); buffed the **Mini Elemental Golem Companion** (`460 HP`, `55 DEF`, `0.55s` i-frames, `+4 HP/s` regen, `35s` respawn, Slam/Bash/Spike rotation); and tuned speed/stamina on Bow, Daggers, **Shadowstrider** (`~+21%` net speed), **Totem of Mist**, and **Totem of Insanity** (`+18%` speed) without slowing heavy armor sets.
* **Persistence, Bestiary & Text Polish**: Fixed Lore Fragment XV (`frag_15`) save/load truncation (`LORE_FRAGMENT_COUNT = 15`), tracked **Frost Revenant** as Bestiary entry #12, and sanitized non-ASCII UI/dialogue typography.

---

## 1. Overview & Lore of Tydeshire

### The Great Unification & The Five Crowns

For more than five centuries, the Land of Tydeshire was consumed by unrelenting feudal warfare. Warlords clashed over barren frontiers, and regional dynasties collapsed as quickly as they arose. Unity remained impossible until **King Charlz** ascended through martial supremacy and iron will, forging an empire from five fractured territories.

Crowned *Charlz the Liege of Kings*, he divided the continent into five sovereign realms to govern peace:

1. **Zyphden** -- Realm of the Gale Winds and high wind-spires.
2. **Azshden** -- Iron cliffs and coastal bastions.
3. **Fleurshden** -- Fertile river valleys and blooming thorns.
4. **Noirden** -- Shrouded obsidian peaks and mists.
5. **Solsden** -- Radiant sun-plains, golden altars, and the High Sanctuary.

> [!NOTE]
> **The Five Crowns Accord**: Each kingdom was chartered with a specific duty to preserve the continental leylines. Together, their five primordial seals held back the ancient void corruption lurking beneath Tydeshire.

### The Fracture & The Long Road

The Pax Charlz endured for three decades before King Charlz succumbed to an unknown creeping madness, dissolving royal assemblies and vanishing into the northern frontiers without an heir. With the High Throne vacant, fear turned brother against brother, border seals fractured, and primeval monstrosities crawled forth from the mire.

The player begins their pilgrimage amidst the cold ashes of the southern sanctuary, armed with little more than instinct, bound north through the corrupted **Crossroads** into the fortified gates of **Zyphden**.

### Lore Fragments Codex (I-XV)

The game world contains 15 discoverable historical fragments documented inside the in-game Lore Journal (`[TAB]` or `[I]` -> Tab 2):

| Fragment ID | Title | In-Game Location | Historical Significance |
| :--- | :--- | :--- | :--- |
| `frag_01` | **The Great Unification** | Ruined Castle Awakening Stele | Chronicles King Charlz uniting the Five Crowns at the High Summit. |
| `frag_02` | **The Shadow over Zyphden** | Eastern Ruins Relic Chest | Documents early rumors of the Lord Liege's madness and lost envoys. |
| `frag_03` | **Zil's Holy Oath** | Ruined Castle High Altar | Records Sun-Crusader Zil's vow as he anchored the southern sanctuary. |
| `frag_04` | **The Five Broken Seals** | Southeast Ruined Ramparts | Details the five elemental warding seals bound directly to the High Crown. |
| `frag_05` | **The Crossroads Watch** | Crossroads Roadside Plinth | Chronicles the ambushed southern patrol when the highway fell dark. |
| `frag_06` | **Rite of the Gale Winds** | Zyphden Northern Wind Lectern | Explains the resonance leylines and the hidden Totem of Sanity. |
| `frag_07` | **Lenon's Field Log** | Zyphden Garrison Armory | Vanguard Captain Lenon's personal reflections on his sworn brother Zil. |
| `frag_08` | **The Sovereign's Burden** | Courtyard Sparring Grounds | King Kreptu's resolve to rally remaining crusaders and hold Zyphden. |
| `frag_09` | **Inscription of the Ancient Pillars** | Royal Keep Throne Pillar | First Warden runes teaching how inner Will strengthens poise against stagger. |
| `frag_10` | **The Royal Armory Archives** | Royal Keep Eastern Vault | Martial treatises on weapon armor penetration and poise-break timing. |
| `frag_11` | **The Frigid Highland War** | North Zyphden Outpost Monolith | Chronicles the ancient highland clash between Charlz and the Troll clans. |
| `frag_12` | **Curse of the Berserker** | Troll Burial Mound (North) | Explains how corrupted subterranean veins warped the Berserker Trolls. |
| `frag_13` | **The Shattered Totem of Blood** | North Zyphden Blood Altar | Warns of the blood frenzy relic trading defense for ferocious power. |
| `frag_14` | **Watch of the Northern Vanguard** | Vanguard Lookout Cairn | A sentry's final entry warning of the beasts howling through the blizzard. |
| `frag_15` | **The Wailing Depths** | Permafrost Crypts Dais | Chronicles the Frost Revenant haunting the subterranean glacial catacombs. |

> [!TIP]
> **Unread Badges & Dialogue History**: Newly uncovered fragments flag the Journal badge with `(!)`. During any NPC or stele dialogue, press `[A]` and `[D]` to flip backward and forward between pages before pressing `[E]` or `[ESC]`.

### Characters & Royal Court

* **King Kreptu (Monarch of Zyphden)**: Sovereign lord presiding over the gale realm from his golden dais, sanctioning courtyard combat trials for worthy champions.
* **Knight Lenon (Vanguard Captain)**: Sworn brother of Lord Zil. An honorable martial master wielding high-velocity lunges and whirlwind cleaves in the Eastern Courtyard.
* **Zil's Remnant (Shadow of the Sun Crusader)**: Spectral guardian of the Ruined Castle's high altar wielding Truth Seeker to test wanderers.
* **Chancellor Malakor, Sentinels Donald & Garrick, Archivist Thaddeus, Lady Genevieve**: The Zyphden Royal Keep court guiding statecraft, archives, and Sovereign Hall security.
* **Tavernkeep Eldon & Tavern Keeper Brena**: Hosts of Hearthside Tavern supplying healing elixirs (Eldon) and specialized combat tonics brewed at the cauldron (Brena).
* **Blacksmith Varek, Outcast Ranger Torvald, Cedric & Maeve**: Frontier artisans, historians, and highland bounty hunters across Zyphden and the Northern Crags.

---

## 2. Campaign Progression & World Design

### Act I: The Ruined Castle

The player awakes unarmed in an overgrown cathedral courtyard:

1. Inspect the central plinth to claim the **Crusader's Broadsword**.
2. Master movement (`[WASD]`), evasive dashing (`[Q]`), parrying (`[F]`), and light/heavy sword combos.
3. Slay the guardian Ruin Sentinel to dissolve the **Sanctuary Ward** sealing the high altar.
4. Confront **Zil's Remnant** at the high dais to win **Truth Seeker (Partial)** + `frag_03`, or accept his mercy to receive **Zil's Token**.
5. Step through the northern gateway to enter the Crossroads.

> [!IMPORTANT]
> **Sanctuary Ward**: You cannot climb to Zil's high altar until the initial Ruin Sentinel is defeated. If Zil proves too lethal early on, accepting his mercy grants **Zil's Token** for safe passage north without blocking story progress.

### Act II: The Crossroads of the Damned

A ruined wilderness highway connecting the southern castle ruins to the northern fortress:

* Ambush patrols of Cultist Slashers, Deathseeker Archers, Void Pyromancers, and Shadow Chargers.
* Hidden shrine in the eastern clearing holding the **Totem of Sanity** (`+15%` Overall Damage, `+15` Will).
* Highway Relic Plinth (`frag_05`) and Crossroads Supply Chest (`+2` Health Potions).
* Northern gateway leading into the Zyphden transition portal.

> [!TIP]
> **Telegraph Clarity**: Melee slashers and chargers project amber/red cones and linear lanes, archers draw red aim lines, and pyromancers trace 3-way landing markers. Sidestep with `[Q]` or reflect missiles back toward your cursor with `[F]`.

### Realm Transition Screen

Between major realm gates, the engine presents a framed dark-stone card with ambient embers and checks `assets/zyphden_transition.png` for optional custom 16:9 concept art. Press `[SPACE]`, `[ENTER]`, or click **Continue** to step through.

### Act III: Fortified Realm of Zyphden

The stone capital of the gale winds features cobblestone plazas, artisan workshops, historical steles (`frag_06`, `frag_07`, `frag_08`), townfolk lore encounters, the Hearthside Tavern, Blacksmith Varek's Forge, and the Royal Keep.

### Blacksmith Varek's Forge

Stationed in the northern artisan quarter (`[E]` at the anvil):

* **4 Dedicated Category Tabs**: Organizes the smithy into four specialized departments:
  * `[1] VANGUARD WEAPONS (9)`: Standard and refined martial armaments across 3 pages.
  * `[2] SACRED WEAPONS (2)`: Consecrated armaments including **Truth Seeker** (Holy Sunblade) and **Sovereign's Scepter** (Restorative Command Rod).
  * `[3] PRIMAL TOTEMS (8)`: Relic empowerment and evolution across all 8 totems (Sanity, Insanity, Mist, Crusader, Elemental Core, Phlegmatic Core, Champion, Commander).
  * `[4] ARMOR FORGE (15)`: Full modular 3-piece sets across Scrap Iron, Basalt Golem, Shadowstrider, Vanguard Bastion, and Frostwarden Attire.
* **Sovereign's Scepter Crafting & Upgrades**: Forged from 14 Ore Scraps, 2 Mist Essences, 2 Shadow Essences, and 120 Shingles. Can be upgraded to Tier +1 (120 Shingles) and Masterwork Tier +2 (240 Shingles) to amplify summon pulse healing and sanctuary radiance.
* **Primal Totem Leveling & Evolution**: Upgrade discovered relics from Tier I (Baseline) to Tier II (Empowered) and Tier III (Masterwork / Evolved Titan) using Shingles, Ore Scraps, and rare essences. Elemental Core Totem evolves into the colossal Commanding Elemental Golem titan with 3 mini golem sub-minions at Tier III.

### Hearthside Tavern, Provisions & Alchemy Workshop

Warm indoor refuge south of the town square:

* **Tavernkeep Eldon**: Initiates Knight Lenon's courtyard summons and sells **Consecrated Health Potions** (`35` Shingles).
* **Tavern Keeper Brena**: Operates the bubbling cauldron station to brew **Stoneskin Tonics** (`[4]`), **Glacial Thaw Draughts** (`[5]`), and **Berserker Brews** (`[6]`).

> [!IMPORTANT]
> **Tavern Story Gate**: Speak with Tavernkeep Eldon inside Hearthside Tavern first to trigger Knight Lenon's entrance and unlock your formal audience with King Kreptu in the Royal Keep.

### Multi-Slot Save Architecture & Storage Vault

* **3-Slot Save Manager (`SaveManager`)**: Saves across `save_slot_1.json` - `save_slot_3.json`, persisting active realm, coordinates, stats, equipped weapon/armor/totem, **3-slot Weapon Hotbar bindings**, **3 persistent equipment loadouts [ I ] [ II ] [ III ]**, weapon tiers, potions/tonics, materials, stash contents, `7` campfire waypoints, `15` lore fragments, `12` Bestiary kill codices, and arena milestones. Auto-saves upon closing the inventory modal.
* **Storage Stash Vault (`[T]`)**: Located at Zyphden Town Square and accessible from all realm campfires via a dual-pane Carried Pouch / Vault Stash transfer modal.

### Expanded Zyphden Royal Keep

Grand stone hall (`1200x1100`) featuring King Kreptu's golden dais, marble colonnades, Royal Archives, Armory Vault (`frag_09`, `frag_10`), and five court NPCs guarding the sovereign crown.

### Act IV: Subterranean Quarry Depths & Primordial Altar

Unlocked after defeating Knight Lenon (`[E]` at the eastern archway):

* Mineral cavern populated by four armored **Ore Golems** (`7.0s` respawn) dropping Ore Scraps for smithing.
* **Primordial Altar (`{ 1000, 320 }`)**: Awakens the **Elemental Golem (Primordial Titan)** boss, which drops the **Elemental Core Totem**, Ore Scraps, Silver Shingles, and Health Potions. Upon defeat, a 120-second (2-minute) dormancy recharge timer begins before the Titan can be summoned again.

### Act V: Restless Mist Forest & Altar of Whispers

Located beyond Zyphden's western ramparts (`1800x1300` twilight woodland), physically gated until Captain Lenon has been defeated in the courtyard spar:

* **Western Archway Gate**: Refuses passage with royal decree feedback and displays `[E] WESTERN MIST GATE (LOCKED - SPAR WITH LENON FIRST)` until Lenon is defeated.
* **Gale Bandits**: Swift dual-dagger ambushers and throwing-knife skirmishers dropping Silver Shingles and **Essence of Mist** (`35%` rate; no Ore Scraps).
* **Altar of Whispers (`{ 1130, 650 }`)**: Offering `2x` Essence of Mist summons **The Gale Archer**, awarding Shadow Essence and a `15%` chance to claim the **Totem of Mist**.

### Act VI: Northern Crags & The Permafrost Crypts

Northern alpine frontier beyond Zyphden's north mountain pass (`2400x1500`), physically gated until Captain Lenon has been defeated in the courtyard spar:

* **Northern Mountain Pass**: Refuses passage with royal decree feedback and displays `[E] NORTHERN PASS (LOCKED - SPAR WITH LENON FIRST)` until Lenon is defeated.
* **Frostbite & Blizzards**: Sub-zero chill slowly builds up (`-25%` move speed and `-40%` stamina regen at `100%`), thawed rapidly beside the Highland Bonfire or mitigated by **Frostwarden Attire** and **Glacial Thaw Draughts**.
* **Outcast Ranger Torvald & Bounties**: Offers three repeatable/tiered hunting contracts (**Frostfang Cull**, **Glacial Shard Harvest**, **Highland Slayer**) beside breakable Frost Crystal nodes.
* **Berserker Troll Warlord**: Summoned via the Blood Altar (`30s` rematch cooldown), guarding a `25%` drop chance for the **Totem of Insanity**.
* **Shrine of Cold Solitude (`{ 1400, 320 }`)**: Bestows the 7th Sacred Relic, **Totem of the Phlegmatic Core**, upon clearing highland bounties.
* **The Permafrost Crypts (`GameState::FROST_CAVERNS`)**: Accessed via the northern chasm entrance (`{ 2200, 260 }`), featuring low-friction black ice inertia, breakable crystal stalagmites, Lore Fragment XV, and the **Frost Revenant** phantom boss.

### Combat Arena & 25-Wave Humoral Gauntlet

Permanently unlocked from the Title Screen, Lobby portal, or Knight Lenon:

* **25-Wave Survival Gauntlet**: Five escalating tiers culling Southern Cultists, Highland Trolls, Unleashed Ore Golems, and recurring Arena bosses (`Wave 5/10/15/20/25`, culminating in the **Crag Berserker Warlord**). Conquering Wave 25 awards the **Grand Apex Victor's Crest**, the **Sovereign's Scepter** (pure healing armament), and the **Commander's Totem** (commanding 7 permanent tower companions with 1:1 enemy procedural sprites).
* **3-Stage Apex Boss Gauntlet**: Accessed directly via Knight Lenon (`[G] APEX BOSS GAUNTLET`), chaining Apex Remnant Zil (2,200 HP, 50 DEF), Apex Primordial Golem Titan (3,400 HP, 75 DEF), and the Apex Berserker Troll Warlord (4,800 HP, 58 DEF) back-to-back with 1.85x-2.0x lethality multipliers, 25% faster windup telegraphs, 30% lower attack recovery, deadlier wave reinforcements, and an intense <35% HP Enrage Phase (+20% speed, -15% windup, -25% recovery, crimson fury aura). Conquering the gauntlet exclusively awards the **Champion's Totem**, +2500 Shingles, +75 Ore Scraps, +10 Mist Essence, +10 Shadow Essence, and the **Apex Gauntlet Warlord Trophy**.
* **Arena Bandage Balance**: Adhesive Bandages have been rebalanced (12% drop rate on enemy kill, max 2 simultaneous active ground bandages, 45s periodic wall dispenser, and zero free initial bandages).

---

## 3. Combat Systems & Mathematical Formulas

### Defense & Armor Penetration Formula

All damage inflicted upon the player or enemies passes through physical armor mitigation:

```text
Mitigated Damage = Raw Damage * [ 100 / (100 + Defense * (1 - Pierce)) ]
```

* **Defense**: Combined helmet/chest/leggings/totem defense or enemy base armor.
* **Pierce**: Armor penetration fraction (e.g., War Mace Earthshaker ignores `25%`, Gale Piercer ignores `50%`, Truth Seeker Sacred Judgment beam ignores `100%`).
* **Minimum Damage**: Any unmitigated hit deals at least `1` damage.

> [!IMPORTANT]
> **Armor vs. Pierce**: Heavily plated foes (Iron Brutes, Ore Golems, Elemental Golem) absorb light sword swings. Use the War Mace, Gale Piercer Drill, Claymore Solar Slam, or timed Parry counters to fracture and bypass their armor.

### Will Stat & Stagger Duration Formula

When parried or hit by high-poise finishers, entities enter a temporary **Staggered** recovery state:

```text
Stagger Duration = Base Stagger * 100 / (100 + Will * 1.25)
```

#### Player Will Scaling
* **Unarmored (Starting)**: `20` Will -> **0.80s** stagger.
* **Crusader Plate**: `65` Will -> **0.55s** stagger.

#### Enemy Will Scaling Upon Parry
* **Cultist Slasher** (`10` Will) -> **1.95s** stagger.
* **Deathseeker Archer** (`15` Will) -> **1.85s** stagger.
* **Void Pyromancer** (`20` Will) -> **1.76s** stagger.
* **Shadow Charger** (`30` Will) -> **1.60s** stagger.
* **Iron Brute** (`50` Will) -> **1.35s** stagger.
* **The Grand Inquisitor** (`60` Will) -> **1.25s** stagger.
* **Zil's Remnant** (`65` Will) -> **1.21s** stagger.
* **Knight Lenon** (`70` Will) -> **1.17s** stagger.

> [!IMPORTANT]
> **Will Stat Mechanics**: Will does not reduce incoming numeric damage; it accelerates your recovery from poise breaks so you can raise guard (`[F]`) or dash (`[Q]`) before follow-up blows land.

### Anti-Stunlock Buffer

Recovering from a player stagger grants **1.8 seconds of Stagger Immunity**. During this grace window, heavy blows deal damage but cannot chain-stagger the player.

### Invulnerability Frames (i-frames)

Activating **Dash (`[Q]`)** triggers a directional burst with **0.25 seconds of invulnerability frames**, allowing melee hits, shockwaves, and projectiles to pass harmlessly while spawning an `EVADED` badge above the player.

### Parry Guard, Counter Buff & Projectile Reflection

Pressing **Parry (`[F]`)** raises a golden cyan guard barrier for **0.24 seconds**:

* **Melee Deflection**: Negates incoming hit damage, staggers the attacker, and grants **+50% Counter-Attack Damage** (`1.5x`) for `2.4s`.
* **Projectile Reflection**: Deflects arrows, void orbs, and beams straight toward your **mouse cursor** with doubled velocity and amplified damage.

### Consumable Health Elixirs, Combat Tonics & Out-of-Combat Regeneration

* **Health Potions (`[H]`)**: Restores **+45 HP** immediately on a **7.0-second cooldown**.
* **Stoneskin Tonic (`[4]`)**: Grants **+18 Defense** (`-10%` movement speed) for `12.0s`.
* **Glacial Thaw Draught (`[5]`)**: Cleanses chill and grants **complete Frostbite immunity** for `15.0s`.
* **Berserker Brew (`[6]`)**: Grants **+30% Attack Damage** (`-15%` Defense) for `10.0s`.
* **Out-of-Combat Regeneration**: Remaining undamaged for **4.0+ seconds** restores **+8.0 HP/s** passively.

### Phlegmatic Composure System & Stoic Poise

The **Phlegmatic Composure** meter (`0.0` to `100.0`) measures focus and poise under pressure:

* **Gaining Composure**:
  * **Timed Parry (`[F]`)**: `+25.0` Composure (`+30.0` total when quenching Choleric frenzies).
  * **Evasive Dash (`[Q]`)**: `+15.0` Composure when dodging through an active hitbox.
  * **Patient Stance**: `+8.0` Composure/s while standing composed near telegraphing enemies without spamming.
* **Losing Composure**: `-25.0` on heavy hits taken, `-15.0` on exhausted heavy attempts, and `-1.5/s` decay out of combat.
* **Composure Tiers**:
  * **Discomposed (`< 30.0`)**: `+15%` stagger duration vulnerability.
  * **Composed (`30.0 - 79.9`)**: Baseline recovery equilibrium.
  * **Stoic Poise (`80.0 - 100.0`)**: Hyper-armor during heavy attack windups and immunity to Sanguine Bleed.
* **Active Ability -- Phlegmatic Trance (`[SPACE]` at `100` Composure)**:
  * **5.0 seconds** of full invulnerability (`TRANCE INVULN!`), zero stamina cost on all actions, and an automatic `120px` projectile reflection aura.

### Medieval Four Humors Alignment

| Humor Type | Element & Color | Combat Temperament | Tactical Vulnerability & Interaction |
| :--- | :--- | :--- | :--- |
| **Choleric** | Yellow Bile (`[255, 140, 40]`) | Relentless aggression, rapid charge velocity, and short windups. | **Quenching**: Parrying inflicts `QUENCHED!` (`-30%` speed for 5.0s, **2.0x stagger duration**, and `+30` Composure). |
| **Melancholic** | Black Bile (`[160, 60, 255]`) | Hardened carapace granting **+50% base armor** and poise. | **Shattering**: War Mace or 2-stage heavies inflict `SHELL SHATTERED!` (`-35%` defense for 4.0s). |
| **Sanguine** | Blood (`[255, 60, 75]`) | Flurry strikes and piercing volleys causing hemorrhage. | **Hemorrhage**: Inflicts Bleed (`10` dmg over `3.0s`); **Stoic Poise (`80+`)** grants Bleed immunity. |
| **Phlegmatic** | Cold Water (`[80, 220, 255]`) | Measured guard stances, chilled strikes, and steady spacing. | **Patience & Poise**: Vulnerable to armor fracture, heavy tremors, and timed ripostes. |

### Combat Juice: Hit-Stop & Directional Camera Shake

Parry deflections, heavy finishers, projectile reflections, and armor fractures trigger a `0.04s - 0.06s` hit-stop freeze paired with directional camera impulse shake along the strike vector.

---

## 4. Controls & Interface Reference

### Keybindings Guide

| Key / Input | Action Name | Gameplay Function |
| :--- | :--- | :--- |
| `W, A, S, D` / `Arrows` | Movement | 8-directional traversal with obstacle sliding. |
| `Mouse Cursor` | Aiming | Reticle aiming for slashes, bows, dashes, and reflected shots. |
| `Left Mouse Button` | Light Attack | Fast slash, thrust, twin-cleaver swing, or Gale Arrow volley. |
| `Right Mouse Button` | Heavy Combo | Weapon signature multi-stage combo or piercing drill shot. |
| `Q Key` | Dash (Evade) | Directional burst with **0.25s i-frames** (`EVADED`). |
| `F Key` | Timed Parry | Active `0.24s` guard window; staggers melee attackers & reflects missiles. |
| `SPACE Key` | Phlegmatic Trance | At `100` Composure: **5.0s invulnerability, 0 stamina cost, 120px auto-reflect aura**. |
| `1, 2, 3 Keys` | 3-Slot Weapon Hotbar | Instantly swap between up to 3 weapons bound in your hotbar slots. |
| `H Key` | Health Potion | Consumes 1 elixir to heal **+45 HP** (`7.0s` cooldown). |
| `4, 5, 6 Keys` | Alchemy Tonics | Drink Stoneskin (`[4]`), Glacial Thaw (`[5]`), or Berserker Brew (`[6]`). |
| `V Key` | Armor Set Skill | Trigger 3/3 armor set skill (`FROST NOVA`, `Dawnfire Aegis`, `Basalt Spires`, etc.). |
| `E Key` / `Enter` | Interact | Speak with NPCs, loot chests, inspect steles, forge, and cross portals. |
| `A Key` / `D Key` | Dialogue Page | Turn dialogue pages backward/forward. |
| `TAB` (`I Key`) | Inventory & Grimoire | Open Equipment Matrix, 3-Slot Hotbar assignment, Lore Journal, Bestiary, and Map. |
| `B Key` | Humoral Bestiary | Open Inventory directly to Tab 3 (Humoral Bestiary & Mastery Perks). |
| `M Key` | Interactive Realm Map | Open frosted tactical map with live player marker and POI labels. |
| `N Key` | Tactical Radar | Toggle corner mini-radar and 8-way cardinal compass ribbon. |
| `T Key` | Stash Vault / Return | Open Storage Stash Vault at town/campfires, or return to Zyphden from Arena. |
| `O Key` | Objectives Tracker | Toggle pinned Chapter 1 quest objectives panel. |
| `C Key` | Controls Guide | Toggle quick-reference keybindings overlay. |
| `ESC Key` | Close / Dismiss | Dismiss open modals, popups, or active dialogues. |
| `F11` / `Alt + Enter` | Fullscreen Toggle | Toggle crisp windowed / native fullscreen modes. |
| `R Key` | Respawn / Retry | Retry active trial or respawn after defeat. |
| `L Key` | Return to Hub | Fast return to Lobby / Armory Sanctuary where supported. |
| `~ Key` (Tilde) | Title Screen | Return to the Title Screen from any state. |

> [!TIP]
> **In-Game Controls Overlay**: Press `[C]` at any time in combat or town to bring up the keybindings overlay without quitting or pausing external menus.

### Modular Visual Action Bar & 3-Slot Weapon Hotbar Dock

* **Bottom-Center Action Bar (`58x62` standard / `64x68` wide)**: Nine framed slots with procedural SVG-style vector silhouettes, keybind tags, stock counters, cooldown wipes, and active buff highlights:
  * **Slots 1-4**: `[Q]` Dash (winged chevron), `[LMB]` Light (active weapon silhouette), `[RMB]` Heavy (empowered cleave silhouette), `[F]` Parry (heraldic shield).
  * **Slots 5-8**: `[H]` Health Flask, `[4]` Stoneskin Tonic, `[5]` Glacial Thaw Draught, `[6]` Berserker Brew.
  * **Slot 9**: `[V]` Armor Skill with dynamic set emblem (`BURST`, `AEGIS`, `CHARGE`, `DECOY`, `SPIKES`, `SPIRES`, `NOVA`) lighting up while wards/buffs remain active.
  * **Trance Medallion**: Centered `[SPACE]` pill above the action bar illuminating at `100` Composure.
* **Bottom-Right 3-Slot Weapon Hotbar Dock (`64x66` / `70x72` slots)**: Framed `114px`-tall card aligned cleanly with the Player Card, displaying the active weapon header (`Name`, `+Tier`, `Light / Heavy DMG`) above `[1]`, `[2]`, and `[3]` procedural weapon icon slots.

### Attributes & Posture Status Indicators

Displayed inside the bottom-left Player Card:

* **DEF / WILL / SHINGLES**: Live defensive mitigation, stagger recovery rating, and Silver Shingles purse.
* **POISE `[DISCOMPOSED / COMPOSED / STOIC POISE]`**: Live Composure tier indicator alongside dynamic posture and buff tray pills (`ICE WARD`, `STAGGERED!`, `+50% COUNTER`, `EVADED`, `BLOOD FRENZY`, `FROSTBITE`).

### In-Game Equipment Matrix, Lore Journal, Bestiary & Realm Map

Opened via `[TAB]` (`[I]` alias), `[B]` (Bestiary), or `[M]` (Realm Map):

* **Tab 1 (`[1]`) -- Equipment, 3-Slot Hotbar & Arsenal**: Inspect weapons and upgrade tiers, assign up to 3 unlocked weapons to Hotbar `[SLOT 1]`, `[SLOT 2]`, and `[SLOT 3]`, toggle saved loadouts (`I / II / III`), inspect armor sets, and equip sacred totems.
* **Tab 2 (`[2]`) -- Lore Journal**: Dual-page medieval book tracking all 15 discovered fragments (`I-XV`) with unread badges (`(!)`).
* **Tab 3 (`[3]` / `[B]`) -- Humoral Bestiary**: Browse 12 hostile archetypes (including **Frost Revenant**), weakness dossiers, and unlock permanent Mastery Hunting Perks at Tier 2 (`5-10 Kills`) and Tier 3 (`12-25 Kills`).
* **Tab 4 (`[4]` / `[M]`) -- Interactive Realm Map**: Cartographic view of Tydeshire with live player marker and POI tooltips.

---

## 5. Weapon Arsenal, Armor & Totems

### Arsenal Summary Table

| Weapon | Light Attack | Heavy Combo | Dash CD | Special Mechanics & Passives |
| :--- | :--- | :--- | :--- | :--- |
| **Unarmed** | None | None | `0.75s` | Starting state in cathedral awakening. |
| **Crusader Blade** | `16 / 21 / 26` dmg (`110-deg`, `15` Stam) | `54 / 66 / 80` dmg (`3-Stage`) | `0.85s` | Stage 1 Crescent Cleave (`+12` Composure), Stage 2 Shock Thrust (interrupts windups), Stage 3 Radiant Sunder (`+50%` vs staggered). |
| **Truth Seeker (Partial)** | `22 / 27` dmg (`130-deg`, `14` Stam) | `70 / 85` dmg (`2-Stage + Beam`) | `0.65s` | Stage 1 Dawnfire Ring (Solar Burn + Holy Beam `24/32`), Stage 2 Sacred Judgment Pillar (`100%` defense pierce + self-cleanse). |
| **Dual Daggers** | `11 / 15 / 20` dmg (`9` Stam) | `42 / 56 / 72` dmg (`3-Stage`) | `0.50s` | `9` stamina per light strike. Critical multipliers (`1.15x / 1.30x`). Heavy chains invulnerable Shadow Puncture (Bleed), Backstab Cross-Slash, and staggering Vault Kick. |
| **War Mace** | `24 / 29 / 35` dmg (`22` Stam) | `74 / 88 / 104` dmg (`2-Stage`) | `1.25s` | Slow high-poise crusher. Heavy executes Skull Crusher (`-20%` Armor Fracture `4s`) into `360-deg` Seismic Tremor (`25%` armor pierce). |
| **Royal Halberd** | `18 / 23 / 28` dmg (`140px`, `14` Stam) | `56 / 70 / 85` dmg (`3-Stage`) | `0.80s` | Extended polearm (`10% / 15% / 20%` armor pierce). Heavy executes i-frame Vault Lunge, `220-deg` cleave, and rapid 3-stab flurry. |
| **Twin Zephyr Daggers** | `15 / 19 / 23` dmg (`8` Stam) | `62 / 75 / 90` dmg (`Shadow Slip`) | `0.35s` | Gale curved blades (`8` stamina per light strike). Heavy executes i-frame Shadow-Step, `360-deg` whirlwind, and explosive shadow decoy. |
| **Windrunner's Recurve** | `18 / 22 / 27` dmg (`18` Stam) | `68 / 80 / 95` dmg (`42` Stam) | `0.50s` | Rapid Gale Arrows (`55px` Windburst AoE at `30%` dmg). Heavy fires Gale Piercer Drill (`50%` pierce, `-40%` Armor Fracture `4s`, `1.2s` stun). |
| **Sun-Forged Claymore** | `26 / 32 / 38` dmg (`140-deg`, `24` Stam) | `88 / 104 / 122` dmg (`2-Stage`) | `1.15s` | Heavy chains Sunfire Crescent (`180-deg` + `4s` Solar Burn) into Solar Cataclysm Slam (`115px` Solar Flare Pillar, `+25%` pierce, `+30%` vs burning). |
| **Storm Glaive** | `20 / 25 / 30` dmg (`155px`, `12` Stam) | `76 / 90 / 106` dmg (`2-Stage`) | `0.70s` | Heavy chains Zephyr Surge Step (`0.18s` i-frame gale corridor) into `360-deg` Tempest Cyclone (`+15` Composure on hit + piercing Crescent Gale Wave). |
| **Twin Glacial Cleavers** | `18 / 23 / 28` dmg (`13` Stam) | `78 / 92 / 108` dmg/hit (`4-Stage`) | `0.85s` | Dual-wielding frost cleavers with fast alternating swings (`0.14s`). Heavy triggers automated 4-stage **Fury of Cleaves** ending in **Glacial Cross-Rupture** (`3.5s` Chill; `175px` Frost Tremor `+35` dmg with Frostwarden). |
| **Sovereign's Scepter** | `4 / 5 / 6` poke dmg (`6` Stam) | `0` enemy dmg (`24` Stam) | `0.60s` | Pure restorative healing armament. Light attack heals targeted companion `+24 / +30 / +38 HP`. Heavy triggers 360-degree **Sanctuary Radiance** (`280px` full AoE heal restoring `+75 / +95 / +120 HP` to all active summons, cleansing debuffs, and granting `+15` DEF and `+20%` speed for `6s`). |

### Detailed Weapon Stances

* **Crusader Blade & Sun-Forged Claymore**: Broad frontal arcs ideal for clearing packs; Sun-Forged Claymore's Stage 2 Solar Cataclysm Slam rewards setting targets ablaze with Stage 1 Sunfire Crescent (`+30%` detonation bonus).
* **Truth Seeker (Partial)**: Upward Dawnfire slashes and `100%` armor-piercing Sacred Judgment lasers cleave entire enemy columns cleanly.
* **Dual Daggers & Twin Zephyr Daggers**: High-tempo skirmishing blades costing `9` / `8` stamina per swing. Manage stamina pacing deliberately and leverage heavy i-frame lunges and shadow decoys to weave around heavy swings.
* **Twin Glacial Cleavers**: Dual-wielded in both hands for brisk alternating slashes and a forward-propelling 4-stage **Fury of Cleaves** barrage that stacks Chill and fractures armored lines.
* **Storm Glaive & Royal Halberd**: Long-reach polearms controlling spacing through linear corridors and `360-deg` cyclone / multi-stab combinations.
* **Windrunner's Recurve**: Mobile skirmish bow pairing rapid Windburst arrows with high-commitment `42`-stamina Gale Piercer Drill shots.

> [!IMPORTANT]
> **Weapon Stamina & Heavy Commitment**: Light daggers and bow arrows now cost meaningful stamina per action (`8`-`9` on Daggers, `18`/`42` on Bow), and heavy combos commit along your cursor trajectory. Open heavy chains during post-parry windows or enemy recovery frames.

### Armor Sets & Frostwarden Synergy

Equipping 3/3 matching pieces awakens both passive traits and an active `[V]` Armor Skill:

* **Commoner's Scrap (`Dust Burst`)**: Low-weight mobility with radial dust stagger.
* **Crusader Plate (`Dawnfire Aegis`)**: Radiant absorb barrier with AoE detonation on expiration/shatter.
* **Vanguard Bastion (`Seismic Charge`)**: `+50` Max HP, hyper-armor, and Halberd/Glaive reach/pierce synergy.
* **Shadowstrider (`Shadow Decoy`)**: Nimble leather mobility (`1.03 / 1.04 / 1.03` pieces with `+10%` set bonus for `~+21%` total speed), `-30%` Dash stamina cost, `+0.10s` Dodge i-frames, `+15%` Crit Chance with Daggers/Bow, and evasive smoke decoy (`25s` cooldown).
* **Forged Scrap Iron (`Ironclad Spike Wall`)**: Reflects `40%` melee damage; Mace armor-fracture synergy.
* **Basalt Golem Carapace (`Basalt Spire Wall`)**: `+35` Def, `70%` knockback resist, periodic heavy absorb, and Sun-Forged Claymore Earth Rupture (`+30` earth dmg).
* **Frostwarden Attire (`FROST NOVA`)**: `+30%` Cold Resistance (`FROST_TROLL`, `BERSERKER_TROLL`, `FROST_REVENANT`, and cold projectiles), Frostbite immunity, `+20%` heavy poise damage, and **Twin Glacial Cleavers Frost Tremor** synergy (`175px` shockwave `+35` frost dmg, `4s` Chill). Pressing `[V]` unleashes a `190px` 12-spire crystalline **FROST NOVA** (`75` frost dmg, `1.2s` non-boss stun, `4.0s` Chill, `-20%` Armor Fracture) plus a `6.0s` `45 HP` **Ice Ward** retaliating against melee attackers for `18` frost dmg and `3.0s` Chill.

### Sacred Totems Compendium

Players can equip one active totem inside `[TAB]` -> Equipment Matrix (`Tab 1`):

| Totem Relic | Acquisition Source | Tier Scaling & Attunement Powers | Drawbacks & Balancing | Visual Glow |
| :--- | :--- | :--- | :--- | :--- |
| **Totem of Sanity** | Crossroads ruined shrine | `+15% / +22% / +30%` Overall DMG, `+15 / +22 / +30` Will | None (Pure Relic) | Emerald Mint (`[100, 235, 180]`) |
| **Totem of Insanity** | Berserker Troll (`25%` drop) | `+65% / +90% / +120%` DMG (`+135% / +180% / +230%` at `<40%` HP), `+18% / +24% / +30%` Move Speed, `+25% / +35% / +45%` Attack Speed | `-50%` Defense, `x1.5` incoming damage | Arterial Crimson (`[255, 45, 60]`) |
| **Totem of Mist** | Gale Archer (`15%` drop) | Mist Fairy (`+20% / +28% / +36%` Crit Chance, `+35% / +50% / +65%` Crit Dmg, `+30 / +45 / +60 HP` Orbs), `+10% / +15% / +20%` Stamina Regen, `+10 / +15 / +20` Will | `-5%` Move Speed, `-15%` Defense, `-25` Max HP | Vapor Azure (`[110, 240, 225]`) |
| **Crusader's Totem** | Knight Lenon (`25%` drop) | `+25% / +35% / +45%` Defense, `+35 / +50 / +65` Max HP, `+18% / +25% / +32%` DMG, Will of Atlas (`4s / 5s / 6s` invuln at `<20%` HP) | `-12%` Move Speed, `-20%` Attack Speed | Radiant Gold (`[255, 215, 80]`) |
| **Elemental Core Totem** | Elemental Golem (Quarry) | **Tier I & II**: Mini Golem (`460 / 650 HP`, `55 / 65 DEF`, `+4 / +6 HP/s` Regen; Slam `36/48 DMG`, Bash `32/42 DMG`, Spike `34/45 DMG`), Bearer Bolt (`54/72 DMG`, `3s` CD).<br>**Tier III Evolution**: Evolved **Commanding Elemental Golem** titan (`920 HP`, `75 DEF`, `28px` body, `+8 HP/s` Regen) which commands and summons **3 orbiting Mini Golems** (`280 HP`, `40 DEF`, `18 DMG`, `35s` auto-respawn). | `-10%` Move Speed, `35s` Companion Respawn | Molten Amber (`[255, 140, 45]`) |
| **Totem of the Phlegmatic Core** | Shrine of Cold Solitude (North) | `+30 / +45 / +60` Max HP, `+25 / +35 / +45` Will, `+20% / +28% / +36%` Defense, `-80%` Knockback, `25% / 35% / +45%` Gray Health recovery, Chills enemies (`3s / 4s / 5s`) on hit & parry | `-8%` Move Speed, `-10%` Stamina Regen | Glacial Cyan (`[140, 220, 255]`) |
| **Champion's Totem** | Apex Boss Gauntlet Victory | `+35% / +45% / +55%` DMG, `+30% / +40% / +50%` Defense, `+15% / +20% / +25%` Move Speed, `+35% / +45% / +55%` Stamina Regen, `-20% / -25% / -30%` Cooldowns, `+40 / +55 / +70` Will, `+50 / +75 / +100` Max HP, `+14.0 / +18.0 / +22.0 HP/s` Out-of-Combat Regen (`+2.0 / +3.0 / +4.0 HP/s` in combat) | Withered Vitality (`-70%` incoming healing from potions, drops, and kill-perks) | Champion Gold (`[245, 200, 60]`) |
| **Commander's Totem** | Royal Combat Arena (Wave 25) | `+20% / +28% / +36%` Defense, `+40 / +55 / +70` Max HP, `+25 / +35 / +45` Will. Commands 7 permanent Tower Gladiators (Slasher, Brute, Charger, Archer, Pyromancer, Stone Golem, Frost Troll) with true enemy HP/DEF and 1:1 exact procedural enemy silhouettes and equipment. Companions maintain loosened formation (`135px` orbit, `90px` personal space repulsion from player). | `-40%` player direct attack damage penalty, `120s` all-must-die squad resurrection countdown | Imperial Purple (`[185, 45, 120]`) |

---

## 6. Bestiary & Boss Encounters

### Hostiles Compendium

| Enemy Name | Type | Def | Will | Telegraph Indicator | Combat Behavior & Strategy |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Cultist Slasher** | Melee Vanguard | `8` | `10` | Amber/Red Cone (`95-deg`, `115px`) | Swift forward slash. Sidestep or parry right before impact for `1.95s` stagger. |
| **Deathseeker Archer** | Ranged Kiting | `6` | `15` | Red Laser Aim Line (`360px`) | Retreats when approached; parry reflects bolts directly back at cursor. |
| **Void Pyromancer** | Occult Caster | `10` | `20` | 3-Way Laser & Landing Flare | Casts tri-directional void orbs; orbs can be reflected cleanly with `[F]`. |
| **Shadow Charger** | Dash Melee | `12` | `30` | Linear Corridor (`50x240px`) | High-speed lunge thrust. Evade laterally (`[Q]`) or parry head-on. |
| **Iron Brute** | Heavy Crusher | `32` | `50` | Expanding Red Circle (`150px` AoE) | High-armor crusher. Heavy mace swings or armor fracture shatter its defense. |
| **Ore Golem** | Melancholic Tank | `45` | `55` | Expanding Red Circle (`135px` AoE) | Quarry mineral guardian (`220 HP`, `45` Def) dropping Ore Scraps on `7.0s` respawn. |
| **Gale Bandit** | Mist Skirmisher | `10` | `18` | Slash Cone & Knife Aim Line | Rapid dual-blade dashes and ranged throwing knives in Restless Mist Forest. |
| **Frost Troll** | Highland Brute | `24` | `45` | Heavy Spiked Club Sweep / Smash | Inflicts chill/frostbite in North Zyphden; mitigated by Frostwarden Cold Resist. |

> [!IMPORTANT]
> **Red Telegraphed Attacks**: Heavy enemy swings marked by expanding red circles, cones, or laser corridors cannot be canceled by light pokes. Dodge out with `[Q]` or time `[F]` parry on impact.

### Boss Encounters & AI Cadence

* **Zil's Remnant (`520 HP`, `28 Def`, `65 Will`)**: Alternates upward radiant slice, `360-deg` Dawnfire tremor, and piercing Holy Beams preceded by a `0.42s` blade gleam (`140-deg` arc, `165px`). Punish only during beam recovery.
* **Knight Lenon (`640 HP`, `32 Def`, `70 Will`)**: Fast `0.48s` lunge thrust and `0.55s` whirlwind cleave. Rematchable in Zyphden courtyard via `[G]` (`30s` cooldown) for Crusader's Totem rolls (`25%`).
* **The Grand Inquisitor (`500 HP`, `26 Def`, `60 Will`)**: Rotates between 12-way radial star bursts, linear beam shards, and Cataclysmic Smite AoEs. Reflect linear shards with `[F]` for massive burst damage.
* **Elemental Golem (`850 HP`, `55 Def`, `90 Will`)**: Four-phase Melancholic Titan in Quarry Depths combining `190px` ground tremors, molten shards, tri-volleys, and 8-way novas. Break armor with War Mace or Claymore Solar Cataclysm.
* **The Gale Archer**: Mist Forest trophy boss firing triple gale volleys and rapid evasive vaults; awards Shadow Essence and `15%` Totem of Mist drop rolls.
* **Berserker Troll Warlord**: Highland apex boss chaining massive spiked club smashes, ground shockwaves, and rage sweeps; awards `25%` Totem of Insanity drop rolls.
* **Frost Revenant**: Permafrost Crypts phantom boss gliding across black ice with 6-way cold `NOVA_STAR` volleys, piercing `BEAM_SHARD` lances (`isCold = true`), and teleport flurries.
* **Apex Boss Gauntlet Champions**: Three escalated pinnacle encounters fought consecutively via Knight Lenon (`[G]`):
  * *Apex Remnant Zil (`2,200 HP`, `50 Def`, `110 Will`, `165 Move Speed`)*: 1.85x damage, snappy 0.32s windups, Phase 4 Solar Cross-Cataclysm beam cross, accompanied by 2 Iron Brutes and 1 Void Pyromancer.
  * *Apex Primordial Golem Titan (`3,400 HP`, `75 Def`, `140 Will`, `145 Move Speed`)*: 1.85x damage, Phase 4 Molten Tremor Ring (230px) and 3-way fan magma geysers, accompanied by 3 Mini Golems and 1 Ore Golem.
  * *Apex Berserker Troll Warlord (`4,800 HP`, `58 Def`, `150 Will`, `235 Move Speed`)*: 2.0x lethal damage, 3-phase melee rotation (Highland Blood Whirlwind, Glacial Cleave Cone, Crag Earth Rupture Slam), accompanied by 2 Frost Trolls and 2 Void Pyromancers.
  * *All Apex Champions*: Feature an unyielding `<35%` HP Enrage Phase (+20% speed, -15% windups, -25% recovery, crimson fury aura with secondary shockwave pulses).

---

## 7. Technical Architecture & Engine Pipeline

### Directory Hierarchy

```text
Phlegmatic/
|-- assets/                     # Custom drop directory for sprites & transition art
|-- bin/                        # Precompiled 64-bit Windows executable
|-- src/
|   |-- main.cpp                # Entry point & main loop lifecycle
|   |-- core/                   # Config, GameConstants, SaveManager, SoundFX, Particles
|   |-- combat/                 # Weapons, Armor, Totems, Inventory, Collision, Projectiles
|   |-- world/                  # TimeOfDay, Map geometry, WaveManager, Campfire, Interactables
|   |-- entities/               # Player, Enemy, PlayerRenderer, EnemyRenderer
|   |-- rendering/              # AssetManager (paper doll & vector icons), FontManager
|   |-- ui/                     # HUD, ModalUI, TitleUI, UI facade
|   `-- states/                 # Game coordinator, Cutscene, Tutorial
|       `-- realms/             # 7 decoupled realm handlers (Castle..FrostCaverns)
|-- vendor/raylib/              # Precompiled Raylib 6.2 headers & static library
|-- build.bat                   # Automated MinGW-w64 compilation script
|-- run.bat                     # Launcher script
|-- CMakeLists.txt              # Cross-platform CMake build configuration
|-- documentation.md            # Complete game & technical reference
`-- VERSIONS.md                 # Full version changelog compendium
```

### State Coordinator Pattern

State coordination is decoupled cleanly across [`Game`](file:///c:/Users/Erick/Phlegmatic/src/states/Game.hpp) and seven dedicated realm controllers inside `src/states/realms/` (`RuinedCastleHandler`, `CrossroadsHandler`, `ZyphdenHandler`, `QuarryHandler`, `RestlessMistForestHandler`, `NorthZyphdenHandler`, `FrostCavernsHandler`) alongside [`WaveManager`](file:///c:/Users/Erick/Phlegmatic/src/world/WaveManager.hpp) for Arena & Gauntlet flow.

### Collision Geometry & Hit Registration

[`CollisionSystem.cpp`](file:///c:/Users/Erick/Phlegmatic/src/combat/CollisionSystem.cpp) handles sector/cone dot-product sweeps, frontal AoE blast circles (Solar Flare Pillar, Glacial Cross-Rupture), oriented beam bounding checks, Chill/Burn/Fracture status propagation, projectile deflections, and Frostwarden Ice Ward melee retaliation.

### GLSL 330 Multiplicative Day-Night Shader Pipeline

[`TimeOfDay.cpp`](file:///c:/Users/Erick/Phlegmatic/src/world/TimeOfDay.cpp) renders environmental atmosphere onto a dynamically resized offscreen `RenderTexture2D` lightmap using embedded GLSL 330 fragment shaders (with bilinear radial texture fallback):

* **Point-Light Attenuation Shader**: Evaluates smooth inverse-square and cubic edge falloff per registered emitter (player hip lantern, bonfires, altars, torches, portals, frost crystals) with subtle flame flicker.
* **Purkinje Composite Tone-Mapper**: Applies a soft highlight shoulder rolloff and cool night Purkinje tint before compositing onto the scene via `BLEND_MULTIPLIED`, preserving clean neutral `WHITE` daylight (`08:30-17:30`), smooth cosine Dawn/Dusk transitions, and rich torchlit islands across night and subterranean realms.

### Procedural Acoustic Audio Synthesis

[`SoundFX.cpp`](file:///c:/Users/Erick/Phlegmatic/src/core/SoundFX.cpp) synthesizes waveforms in memory via Raylib audio buffers (cathedral bells, metallic sword slashes & parry clangs, heavy impacts, howling blizzards, snow crunch footsteps, portcullis rumbles) without requiring external `.wav` dependencies.

### Real-Time Modal Background Cooldown Engine

[`Game::UpdateBackgroundCooldowns`](file:///c:/Users/Erick/Phlegmatic/src/states/Game.cpp) and [`Player::UpdateBackgroundTimers`](file:///c:/Users/Erick/Phlegmatic/src/entities/Player.cpp) decouple world time progression from modal display states. When viewing Inventory, Bestiary, Map, Storage Stash, or Controls modals, background clocks continuously tick:
* **Boss Dormancy & Rematch Timers**: Elemental Golem 120s altar recharge, Knight Lenon rematch timer, and Berserker Troll blood altar cooldown.
* **Companion Resurrection Countdown**: Conqueror Squad 120s all-7-must-die squad resurrection timer and Elemental Golem companion reconstruction.
* **Player Attributes & Consumables**: Health potion cooldowns, armor set skill cooldowns, active buff decay, and passive out-of-combat regeneration.
* **Diurnal Atmosphere & Spawners**: Day/Night celestial cycle and arena bandage drop timers.

### Custom Asset Drop Pipeline

Assets dropped under `assets/` load automatically at launch without recompilation (`assets/zyphden_transition.png` for custom transition artwork, optional sprite sheet / tileset overrides).

---

## 8. Building & Setup Guide

### Automated MinGW-w64 Batch Script

```powershell
.\build.bat
```

Compiles all 31 `.cpp` translation units with `-std=c++17 -O2 -static` against static Raylib 6.2 and writes both `phlegmatic.exe` and `bin\phlegmatic.exe`.

### CMake & Ninja Build

```powershell
cmake -B build -G Ninja
cmake --build build
.\build\phlegmatic.exe
```

### Precompiled Executables

Run `.\phlegmatic.exe`, `.\bin\phlegmatic.exe`, or `.\run.bat` directly with zero external runtime dependencies.

---

## 9. Uncharted Realms Beyond Zyphden

While Zyphden and its surrounding highlands, quarry depths, mist forests, and permafrost crypts form the playable campaign frontier, the remaining four sovereign Crowns of Tydeshire frame future realm expansions:

* **Azshden**: Iron coastal cliffs and fortified naval bastions in the western seas.
* **Fleurshden**: Corrupted fertile river valleys and thorn-bound aqueducts.
* **Noirden**: Shrouded obsidian peaks, shadow citadels, and lingering void rifts.
* **Solsden**: Sunlit high plateaus ascending toward King Charlz's High Sanctuary.
