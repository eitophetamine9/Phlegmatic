# PHLEGMATIC - Version History & Patch Notes Compendium

*The complete changelog, system updates, combat balance revisions, and interface overhauls across development milestones for **Phlegmatic**.*

---

## Table of Versions

- [Version 0.0.1k - 3-Slot Weapon Hotbar, Enlarged Icon HUD, GLSL Lighting & Combat Reworks](#version-001k---3-slot-weapon-hotbar-enlarged-icon-hud-glsl-lighting--combat-reworks)
- [Version 0.0.1j - Combat Executions, Alchemy Workshop, Phlegmatic Core & Caverns](#version-001j---combat-executions-alchemy-workshop-phlegmatic-core--caverns)
- [Version 0.0.1i - Northern Arsenal, Highland Atmosphere, Outcast Bounties & Apex Boss Gauntlet](#version-001i---northern-arsenal-highland-atmosphere-outcast-bounties--apex-boss-gauntlet)
- [Version 0.0.1h - Northern Crags Expansion, Berserker Troll Boss & Totem of Insanity](#version-001h---northern-crags-expansion-berserker-troll-boss--totem-of-insanity)
- [Version 0.0.1g - Equipment Overhaul, Crusader Totem & Golem Synergy](#version-001g---equipment-overhaul-crusader-totem--golem-synergy)
- [Version 0.0.1f - Arena Milestones, Soundscapes & Combat Polish](#version-001f---arena-milestones-soundscapes--combat-polish)
- [Version 0.0.1e - Master Forge, Heavy Arsenal & Subterranean Depths](#version-001e---master-forge-heavy-arsenal--subterranean-depths)
- [Version 0.0.1d - Input Architecture, Royal Halberd & Tactical HUD](#version-001d---input-architecture-royal-halberd--tactical-hud)
- [Version 0.0.1c - Interactive Realm Cartography & Quarry Mines](#version-001c---interactive-realm-cartography--quarry-mines)
- [Version 0.0.1b - Typography, Multi-Slot Persistence & Economy](#version-001b---typography-multi-slot-persistence--economy)

---

## Version 0.0.1k - 3-Slot Weapon Hotbar, Enlarged Icon HUD, GLSL Lighting & Combat Reworks

### Major Features & Bug Fixes Implemented

#### 1. Modal Navigation Repairs & `[TAB]` Inventory Restoration
* **Realm Map `[M]` Same-Frame Dismissal Fix**: Removed duplicate `IsKeyPressed(KEY_M)` / `KEY_ESCAPE` close checks inside `ModalUI::DrawMapModal` and `ModalUI::DrawInventory` so `[M]` keeps the Realm Map open cleanly until toggled off by `Game::Update`.
* **Humoral Bestiary Access & `[B]` Shortcut**: Fixed `Game.cpp` tab-switch redirection (`inventoryActiveTab == 3` instead of `2`) so clicking **Bestiary** or pressing `[3]` inside Inventory no longer jumps to the Realm Map, and added a direct `[B]` hotkey to jump straight to Bestiary.
* **Top-Right HUD Click Parity**: Extended mouse click handling for the Map, Inventory, and Controls HUD pills across `RESTLESS_MIST_FOREST`, `NORTH_ZYPHDEN`, and `FROST_CAVERNS`.
* **`[TAB]` Inventory Restoration**: Removed the old `[TAB]` sequential weapon-cycling binding across `Game.cpp`, `ZyphdenHandler`, and `NorthZyphdenHandler`, restoring `[TAB]` as the primary Inventory toggle (`[I]` remains supported as a secondary alias).

#### 2. 3-Slot Weapon Hotbar (`[1]`-`[3]`) & Rebound Quick Belt (`[H]` / `[4]`-`[6]`)
* **3-Slot Arsenal Hotbar**: Added `weaponHotbar[3]` and `activeWeaponSlot` inside `Inventory`, with Inventory drawer `[SLOT 1]`, `[SLOT 2]`, and `[SLOT 3]` assignment controls, numbered badge indicators, automatic assignment of newly unlocked weapons into empty slots, and instant combat switching on `[1]`, `[2]`, and `[3]`.
* **Save Persistence**: Serialized `weaponHotbar` and `activeWeaponSlot` in `SaveManager` with backward-compatible seeding when loading legacy pre-`0.0.1k` saves.
* **Consumable Key Rebindings**: Moved Quick Belt elixirs off `[1-3]` cleanly onto **`[H]`** (Consecrated Health Potion), **`[4]`** (Stoneskin Tonic), **`[5]`** (Glacial Thaw Draught), and **`[6]`** (Berserker Brew) across gameplay logic, HUD prompts, Apothecary Brena's shop, and the Controls overlay.

#### 3. Enlarged Vector-Icon Action Bar & 3-Slot Weapon Hotbar Dock
* **Enlarged Action & Potion Slots**: Scaled bottom-center slots from `46x46` to `58x62` (`64x68` at >=1500px width) with custom procedural vector silhouettes inside every slot (`DASH`, dynamic active `LIGHT`/`HEAVY` weapon icons, heraldic `PARRY`, custom flasks for `HEAL`/`STONE`/`THAW`/`BERSERK`, and set-specific `SKILL [V]` emblems), cooldown wipe animations, and active Armor Skill highlights.
* **Bottom-Right 3-Slot Weapon Dock**: Replaced the small single-weapon readout across Lobby, Combat, and Campaign HUDs with a framed `114px`-tall dock matching the bottom-left Player Card, showing active weapon name/upgrade tier/`L & H` damage above three `64x66` (`70x72` wide) procedural weapon icon slots (`[1]`, `[2]`, `[3]`).
* **Arena Hint Alignment**: Raised `[T] RETURN TO REALM` above the bottom-left Player Card to eliminate overlap with `DEF / WILL`.

#### 4. GLSL 330 Multiplicative Day-Night Shader & Realm Light Emitters
* **Offscreen Lightmap & Shaders**: Overhauled `TimeOfDaySystem` to compose scene lighting via a dynamically resized offscreen `RenderTexture2D`, an embedded GLSL 330 inverse-square cubic attenuation point-light shader (with bilinear radial fallback), and a shoulder-mapped Purkinje composite shader applied via `BLEND_MULTIPLIED`.
* **Natural Diurnal & Subterranean Profiles**: Eliminated flat purple washes and stray star pixels on terrain, tuning crisp Day neutrality (`08:30-17:30`), cosine Dawn (`05:00-08:30`) / Dusk (`17:30-20:30`) transitions, moonlit Night (`20:30-05:00`), warm Tavern/Keep interiors, and subterranean ambient profiles for Quarry Depths & Frost Caverns.
* **Full Realm Emitter Registration**: Registered campfires, sacred altars, torches, crystals, and glowing braziers every frame across all realm handlers.

#### 5. Removal of Stagger-on-Every-Hit Loop & Visceral Execution
* **Removed Broken Stunlock Loop**: Removed `[E]` Visceral Execution deathblows, `Adrenaline Surge`, `IsVulnerableToExecution()`, and spinning execution reticles across `CollisionSystem`, `Player`, `Enemy`, `EnemyRenderer`, `HUD`, and `ModalUI`.
* **Totem of the Phlegmatic Core Adjustment**: Replaced per-hit `enemy.ApplyStagger(0.35f)` with `enemy.ApplyChill(3.0f)` while preserving Gray Health counter-recovery and `-80%` knockback resistance.
* **Bestiary Tier 3 Perk Updates**: Updated *Troll Slayer T3* to restore `+25 HP` on heavy finisher hits/kills and *Cultist Purifier T3* to restore `+15 Stamina` & `+10 Composure` on cultist kills.

#### 6. Weapon Reworks: Twin Glacial Cleavers, Sun-Forged Claymore & Storm Glaive
* **Twin Glacial Cleavers (`WeaponType::GLACIAL_CLEAVER`)**: Transformed into high-tempo dual-wielding frost cleavers rendered in both player hands (`0.14s` light duration, `0.20s` cooldown, `13` stamina, `20 / 25 / 31` alternating slashes) with a **4-stage automated "Fury of Cleaves"** heavy combo ending in **Glacial Cross-Rupture** (`46 / 54 / 64` dmg + `3.5s` Chill + Frost Tremor synergy at `175px` for `+35` dmg when wearing Frostwarden).
* **Sun-Forged Claymore Heavy ("Solar Cataclysm")**: Reworked heavy attack into a 2-stage greatsword combo: **Stage 1 Sunfire Crescent** (`180 deg` sweep inflicting `4.0s` Solar Burn DoT) into **Stage 2 Solar Cataclysm Slam** (`115px` forward Solar Flare Pillar with `+25%` defense pierce, `+30%` vs burning targets, and Basalt Golem Earth Rupture synergy).
* **Storm Glaive Heavy ("Tempest Cyclone")**: Reworked heavy attack into a 2-stage wind polearm combo: **Stage 1 Zephyr Surge Step** (`0.18s` i-frame gale lunge corridor) into **Stage 2 Tempest Cyclone** (`360 deg` `180px` gale vortex restoring `+15` Composure on hit and firing a piercing **Crescent Gale Wave** projectile).

#### 7. Frostwarden Set Revamp (`FROST NOVA`, Ice Ward & Cold Resist)
* **Enemy Chill Status**: Added `Enemy::ApplyChill` (`-40%` movement speed, `30%` slower windups, icy cyan rim visuals & frost particles).
* **`FROST NOVA` Active Skill (`[V]`)**: Renamed from `Glacial Frost Nova` to fit cleanly in the HUD; unleashes a `190px` 12-spire crystalline nova dealing `75` frost dmg, `1.2s` non-boss stun, `4.0s` Chill, `-20%` Armor Fracture, and grants a `6.0s` `45 HP` **Ice Ward** that retaliates against melee attackers (`18` frost dmg + `3.0s` Chill) and persists full duration for retaliation even if absorbed.
* **Functional `Glacial Tenacity` Passives**: Hooked actual `30% Cold Resistance` against frost trolls and cold projectiles (`Projectile::isCold`) and `+20%` heavy poise damage multiplier across all weapons.

#### 8. Stamina & Relic Balance Adjustments
* **Bow & Daggers Stamina**: Removed the 5-hit free-swing bypass on daggers (`Dual Daggers`: `9` light / `28` heavy / `16` dash; `Twin Zephyr Daggers`: `8` light / `25` heavy / `14` dash) and raised `Windrunner's Recurve` arrow cost (`18` light / `42` heavy stamina).
* **Totem of Mist Tuning**: Tuned movement speed multiplier to `0.95x` (`-5%`), removed cooldown haste (`1.0x`), and adjusted stamina regeneration to `+10%` while preserving Mist Fairy crits (`+20%` chance, `+35%` dmg) and healing orbs (`+30 HP`).

#### 9. Companion Buffs, Speed Rebalance & Persistence Polish (`ver.0.0.1k` Follow-up)
* **Save & Bestiary Progression Fixes**: Raised `LORE_FRAGMENT_COUNT` (`14` -> `15`) so Fragment XV (`frag_15`) persists properly in save slots, and raised `BESTIARY_COUNT` (`11` -> `12`) to track **Frost Revenant** (`Phantasma Glacialis`) kills alongside northern Frost/Troll Slayer perks.
* **Empowered Mini Elemental Golem Companion**: Raised companion durability to `460 HP` and `55 DEF` with `+4 HP/s` self-regeneration, `0.55s` hit invulnerability frames (preventing multi-frame boss strike shreds), faster movement (`215 px/s`), `35s` shatter respawn, and a 3-pattern attack rotation (**Heavy AoE Slam** `36 DMG` / `80px`, **Forward Bash Knockback** `32 DMG` + Armor Fracture, and **Elemental Spike Throw** `34 DMG` piercing projectile up to `280px`).
* **Shadowstrider & Totem Speed Rebalance**: Tempered Shadowstrider piece modifiers (`1.03 / 1.04 / 1.03`) and full-set speed (`1.25x -> 1.10x`) for a balanced `~+21%` nimble feel, reduced **Totem of Insanity** speed (`+35% -> +18%`), and eased **Basalt Golem**'s extra set penalty (`0.90x -> 0.96x`) so heavy sets stay responsive.
* **UI Text Hygiene**: Normalized non-ASCII smart quotes, em-dashes, and bullets in rendered UI, dialogue, and lore strings to pure ASCII for crisp Raylib bitmap rendering.

#### 10. Persistent Equipment Loadouts, Stage 7 Crypts Waypoint & Champion's Totem
* **Persistent Equipment Loadout Snapshots**: Fully serialized `loadouts[3]` (weapons, helmets, chestplates, leggings, totems, configured flags) and `activeLoadoutIdx` into `GameSaveData` and JSON save files with backward-compatible array parsing. Added auto-saving on closing the inventory modal (`[TAB]` / `[ESC]`) so snapshots taken via `[SAVE]` and equipment adjustments persist immediately across game reboots.
* **Stage #7 Permafrost Crypts Waypoint & Bonfire 7 Parity**: Raised `CAMPFIRE_COUNT` from `6` to `7` in `GameConstants.hpp` and `SaveManager.hpp` so the subterranean Permafrost Crypts bonfire unlocks and saves properly; added `STAGE #7: PERMAFROST CRYPTS` with diamond waypoint, connecting leylines, active stage tracking, and fast-travel execution to `EnterFrostCaverns()` on the Realm Map.
* **Champion's Totem (8th Sacred Relic, 7th Attunable)**: Added the `CHAMPION_TOTEM` awarded for conquering the Royal Combat Arena (Wave 25 or Apex Gauntlet). Features massive all-stat enhancements (`+35% Damage`, `+30% Armor Defense`, `+15% Movement Speed`, `+35% Stamina Regen`, `-20% Cooldowns`, `+40 Will`, `+50 Max HP`) and surged passive health regeneration (`+14.0 HP/s` out-of-combat, `+2.0 HP/s` continuous in-combat) counterbalanced by *Withered Vitality* (`-70%` incoming healing from external potions, drops, and kill-perks).
* **Combat Tactility & QoL Polish**: Added metallic unsheathe audio when equipping weapons via hotbar keys (`[1]`, `[2]`, `[3]`), enabled mouse-click weapon swapping directly on the HUD hotbar dock cards with hover highlights and swing suppression, cleared pending attack windups on weapon swap, and tuned Frost Revenant's melee cone windup from `0.50s` to `0.62s` for fair dodge reaction windows.

---

## Version 0.0.1j - Combat Executions, Alchemy Workshop, Phlegmatic Core & Caverns

### Major Features Implemented

#### 1. Visceral Execution & Stagger Deathblow System
* **Poise-Break Vulnerability**: When enemy poise is shattered and enemy is stunned, an animated diamond reticle signals visceral deathblow readiness.
* **Visceral Lunge & Deathblow**: Pressing `[F]` within 120px lunges into the staggered target with full i-frame invulnerability, executing a fatal critical strike scaled to equipped weapon tier.
* **Adrenaline Surge Buff**: Successful execution grants 4.0s of +25% attack speed and +15% movement speed with golden haste trails.

#### 2. Alchemy Workshop & Quick-Item Belt
* **Tavern Keeper Brena Brewing Station**: Interactive alchemy cauldron at Hearthside Tavern in Zyphden.
* **Combat Tonics**:
  * Stoneskin Tonic: +18 Defense, -10% speed for 12.0s.
  * Glacial Draught: Complete frostbite immunity and cold aura for 15.0s.
  * Berserker Draught: +30% attack damage, -15% defense for 10.0s.
* **Quick-Item Belt**: Number keys `[1]`, `[2]`, `[3]` allow instantaneous combat consumption during encounters.

#### 3. Totem of the Phlegmatic Core (7th Sacred Relic)
* **Phlegmatic Apathy**: Decreases incoming knockback by 80%, providing stoic stance against heavy impacts.
* **Gray Health Regeneration**: 50% of incoming damage converts to recoverable gray health, restored upon landing counter-attacks within 3.5 seconds.
* **Chill Counter**: Successful parries emit a freezing shockwave that inflicts frostbite and slows nearby enemies.
* **Shrine of Cold Solitude**: Altar located at `{ 1400.0f, 320.0f }` in North Zyphden, claimable after Torvald bounties.

#### 4. Dynamic Time-of-Day Lighting Cycle & Nightfall Incursions
* **Celestial Clock**: 24-hour day cycle with Dawn, Day, Dusk, and Night phases interpolated smoothly across all outdoor realms.
* **Point-Light Sources**: Player hip lantern, campfires, torches, altars, and glowing braziers cut through darkness with soft radial lighting.
* **Nightfall Incursions**: Nocturnal elite monsters spawn with purple void auras, boosted stats, and doubled essence and loot yields.
* **Celestial Clock Dial**: Integrated into location banner with sun/moon glyphs and formatted time (`HH:MM`).

#### 5. Tactical Corner Mini-Map Radar & Compass HUD
* **360-Degree Sweep Radar**: Mini-radar HUD displaying player heading, POIs (campfires as golden diamonds, NPCs as emerald circles, altars as cyan diamonds, regular enemies as red dots, bosses as pulsing crimson rings, elites as purple pips).
* **8-Way Cardinal Compass Tape**: Heading ribbon showing cardinal directions (`[N] NW 315'`), toggleable with `[N]`.

#### 6. Bestiary Mastery Hunting Perks & Codex Seals
* **Hunting Mastery Codex Seals**: Bestiary entries award Tier 1 (`5 Kills`), Tier 2 (`15 Kills`), and Tier 3 (`35 Kills`) mastery seals.
* **Active Combat Perks**:
  * Troll Slayer: +25% frostbite resistance, +20% damage to trolls, and +25 HP restored on troll execution.
  * Golem Breaker: +25% armor pierce and +1 bonus scrap yield from mining nodes.
  * Gale Hunter: +15% stamina regeneration rate and +20% damage against mist rogues.
  * Cultist Purifier: +15% damage and +25% critical strike multiplier against cathedral cultists.

#### 7. Subterranean Highland Caverns (The Permafrost Crypts)
* **The Permafrost Crypts Realm**: New subterranean dungeon realm (`GameState::FROST_CAVERNS`, `MapEnvironment::FROST_CAVERNS`) accessed through the highland chasm cavern mouth at `{ 2200.0f, 260.0f }`.
* **Icy Floor Inertia & Gliding**: Low friction black ice floor mechanics with smooth momentum acceleration and frosty drift trails.
* **Breakable Stalagmites**: 7 crystalline stalagmites shattered by weapon attacks to yield iron scrap, silver shingles, and essence.
* **The Frost Revenant Boss**: Ethereal chasm phantom wielding a frost scythe with radial ice novas, piercing glacial lances, and teleport flurries.
* **Lore Fragment XV**: "Fragment XV: The Wailing Depths of the Phlegmatic Chasm" preserved on the inner crypt sarcophagus dais.

#### 8. Storage Stash Chest & Multi-Bonfire Persistence
* **Ancient Vanguard Stash Trunk**: Dedicated persistent storage trunk in Zyphden Town Square and accessible at all realm campfires via `[T]`.
* **Dual-Pane Stash Modal**: Side-by-side Carried Pouch and Vault Stash interface with single-item and bulk deposit/withdraw actions.
* **Full Persistence**: Stash quantities for shingles, iron scraps, frost shards, mist essence, shadow essence, and all elixirs serialized to save slots.

#### 9. File Architecture Restructuring & Subsystem Decoupling
* **Decoupled Realm Subsystem (`src/states/realms/`)**: Migrated all 7 realm environment coordinators (`RuinedCastleHandler`, `CrossroadsHandler`, `ZyphdenHandler`, `QuarryHandler`, `RestlessMistForestHandler`, `NorthZyphdenHandler`, `FrostCavernsHandler`) into an isolated `realms/` subsystem directory.
* **Compilation & Build Automation**: Synchronized `build.bat` MinGW GCC compilation flags with `-Isrc/states/realms` and mapped source paths.
* **CMake Configuration**: Updated `CMakeLists.txt` to automatically discover and compile all headers and source files under `src/states/realms/`.
* **Header Inclusions**: Refactored `Game.hpp` includes to utilize standardized `realms/` relative imports, decoupling core state management from environmental encounter logic.

### In-Development Roadmap
1. [COMPLETED] Visceral Execution & Stagger Deathblow System
2. [COMPLETED] Alchemy Workshop & Quick-Item Belt
3. [COMPLETED] Totem of the Phlegmatic Core
4. [COMPLETED] Dynamic Time-of-Day Lighting Cycle & Nightfall Incursions
5. [COMPLETED] Tactical Corner Mini-Map Radar & Compass HUD
6. [COMPLETED] Bestiary Mastery Hunting Perks
7. [COMPLETED] Permafrost Crypts Subterranean Caverns
8. [COMPLETED] Storage Stash Chest & Persistence
9. [COMPLETED] File Architecture Restructuring & Subsystem Decoupling: Decoupled realm handlers into src/states/realms/ and synchronized build configurations.

---

## Version 0.0.1i - Northern Arsenal, Highland Atmosphere, Outcast Bounties & Apex Boss Gauntlet

### 1. Northern Arsenal: Glacial Cleaver & Frostwarden Armor Set
* **Glacial Spiked Cleaver Colossal Weapon**:
  * Added `WeaponType::GLACIAL_CLEAVER` colossal two-handed great-axe (+0 Base, +1 Forged, +2 Masterwork tiers).
  * High-tier base damage (52 light / 88 heavy base, scaling up to 72 light / 120 heavy at tier +2).
  * Unique heavy attack mechanic: Unleashes wide-arc seismic Frost Tremor shockwaves (+45 physical & cold damage) that stagger approaching enemies.
  * Innate unbreakable hyper-armor during attack windups, preventing poise interruption when trading blows with heavy foes.
  * Custom procedural vector renderer in `AssetManager::DrawCleaver` with detailed rune-engraved broadhead blade, ice crystal core, and spiked counterweight pommel.
* **Frostwarden Highland Armor Set**:
  * Added `ArmorSet::FROSTWARDEN` with three modular pieces:
    * `HelmetType::FROSTWARDEN_HOOD` (+12 Defense, +10 Max HP, +5% Stamina Regen).
    * `ChestplateType::FROSTWARDEN_HAUBERK` (+28 Defense, +25 Max HP, 0.95x Speed).
    * `LeggingsType::FROSTWARDEN_GREAVES` (+18 Defense, +15 Max HP, 0.95x Speed).
  * **Glacial Tenacity Set Bonus (Passive)**:
    * Full 3-piece set grants +40 Max HP, unbreakable hyper-armor across all weapon attacks, and total Frostbite cold immunity in northern sub-zero climates.
  * **Glacial Frost Nova (Active Set Skill - [V] Key)**:
    * Triggers a devastating 160px frost shockwave dealing 55 cold damage, 1.8s freeze stun, and 15 composure break on a 30s cooldown.
* **Blacksmith Varek Master Forge Expansion**:
  * Integrated all 4 new northern equipment pieces into Blacksmith Varek's Forge Crafting Matrix in Zyphden with authentic material costs (Ore Scraps and Silver Shingles).

### 2. Highland Atmosphere, Procedural Audio & Frostbite System
* **Procedural Acoustic Audio Generation (SoundFX)**:
  * Implemented procedural howling glacial blizzard ambience in `SoundFX::GenerateAmbienceBlizzard()`, synthesizing sub-bass wind buffeting (70-120Hz) and swept resonant whistling gusts (1200-2200Hz).
  * Integrated dedicated ambient track loop for `GameState::NORTH_ZYPHDEN` with dynamic boss ducking interpolation.
  * Implemented procedural snow crunch footstep sound effects in `SoundFX::GenerateSnowCrunch()` triggered periodically while moving through snowy highland terrain, paired with soft snow dust puffs.
* **Dynamic Blizzard Particle System**:
  * Added drifting blizzard snowfall stream in `NorthZyphdenHandler`, featuring 160 snowflakes sweeping across the camera view with wind velocity and dynamic view-boundary wrapping.
* **Frostbite Status Effect & Bonfire Warmth Thawing**:
  * Moving through the sub-zero highland winds gradually accumulates a Frostbite chill meter (+5% per second outdoors).
  * Resting or standing near the Highland Crags Bonfire ({ 1200.0f, 1300.0f }) rapidly warms and thaws the player (-25% per second; full rest cleanses instantly).
  * Equipping the full Frostwarden armor set grants complete cold immunity, preventing chill accumulation.
  * Upon reaching 100%, the player contracts **Frostbite**: movement speed is reduced by 20% and stamina regeneration is slowed by 25%.
  * Added `[FROSTBITE: -20% SPD, -25% STAM REGEN]` and `[CHILL: X%]` status pills in the action bar buff tray.
  * Added a dynamic frostbite screen-edge vignette that pulses with icy blue borders when exposed to extreme cold.
* **Breakable Frost Crystal Mineral Nodes**:
  * Placed 4 breakable frost crystal deposit spires across northern ridges ({ 480, 750 }, { 1920, 750 }, { 800, 520 }, { 1600, 520 }).
  * Striking deposits with melee attacks damages and shatters the crystals, rewarding 1-2 Ore Scraps and 15-30 Silver Shingles with 45s respawn timers.

### 3. Outcast Ranger Torvald NPC & Highland Hunting Bounties
* **Ranger Torvald NPC**:
  * Positioned near the Highland Crags Bonfire ({ 1115.0f, 1295.0f }) with custom vector rendering (hunter cloak, fur-lined cowl, recurve hunting bow, and hip quiver).
  * Overhead status badges reflecting contract status: `[!] BOUNTY CLAIMABLE`, `[?] HIGHLAND BOUNTIES`, and `RANGER TORVALD`.
* **Three Tiered Hunting Bounties**:
  * **Bounty I: Frostfang Cull**: Slay 5 Highland Frost Trolls (Reward: 8 Ore Scraps, 250 Silver Shingles).
  * **Bounty II: Glacial Shard Harvest**: Mine 3 Highland Frost Crystals (Reward: 12 Ore Scraps, 350 Silver Shingles).
  * **Bounty III: Highland Slayer**: Vanquish the Berserker Troll Warlord (Reward: 25 Ore Scraps, 800 Silver Shingles).
* **Bounty Tracking & Paginated Dialogue**:
  * Real-time in-game progress tracking with celebratory completion banners and chime audio upon achieving contract milestones.
  * Integrated multi-page dialogue system in `GameUI::DrawNPCDialogueBox` enabling contract review and claiming rewards directly from Torvald.

### 4. Combat Polish & Quick Weapon Swap Hotkey
* **Quick Weapon Swap Hotkey ([TAB] Key)**:
  * Implemented instant mid-combat weapon swapping via `[TAB]` hotkey, seamlessly cycling through all unlocked weapons in canonical progression without pausing or opening the inventory modal.
  * Triggers scabbard draw audio effects, golden weapon swap text banners, and spark particles.
  * Updated modal controls guides in `ModalUI.cpp`: dedicated `[TAB]` quick swap and `[I]` inventory mapping.

### 5. Knight Lenon Apex Boss Gauntlet Mode
* **Consecutive Boss Rush Gauntlet**:
  * Knight Lenon now offers an optional "Apex Boss Gauntlet" mode in the Royal Courtyard of Zyphden.
  * Challenges the player with three consecutive boss encounters with no breaks or resting between rounds:
    * **Stage I**: Remnant Zil (Holy Champion) supported by 2 Iron Brutes.
    * **Stage II**: Primordial Elemental Golem Titan supported by 2 Mini Elemental Golems.
    * **Stage III**: The Berserker Troll Warlord supported by 2 Frost Trolls.
* **Dual-Choice Dialogue Interface**:
  * Lenon's arena prompt features side-by-side interactive buttons: `[R] COMBAT ARENA (25 WAVES)` and `[G] APEX BOSS GAUNTLET (3 STAGES)` with keyboard shortcuts and mouse clicks.
* **Apex Gauntlet Triumph Rewards**:
  * Conquering all 3 stages awards 40 Ore Scraps, 1200 Silver Shingles, 4 Essence of Mist, 4 Shadow Essence, and the prestigious `item_boss_gauntlet_trophy` (Apex Gauntlet Warlord Trophy).

---

---

## Version 0.0.1h - Northern Crags Expansion, Berserker Troll Boss & Totem of Insanity

### 1. Fix Tutorial Area Fast Travel Inventory Retention
* **Preserved Arsenal & Stats on Re-entry**:
  * Fixed an issue where fast traveling to Stage #1 (Ruined Castle) wiped out collected player armaments, armors, materials, and character stats.
  * Refactored `Game::EnterRuinedCastle(bool isInitialNewGame = false)` to distinguish initial game startup from subsequent fast travel, death revivals, and game loads.
  * Preserved active weapons, modular armor loadouts, attunable totems, shingles, and upgrade levels.
  * Made the tutorial castle accessible anytime so players can return to explore missed lore steles without penalty. Missed lore fragments (`frag_01` to `frag_04`, `frag_09`) remain interactable and unlockable upon re-entry.

### 2. Fix ModalUI Equipment Text Overlapping
* **Dynamic Set Bonus & Armor Description Flow**:
  * Fixed typography collisions in `ModalUI.cpp` where switching armor loadouts caused set bonus descriptions to overlap with set skill headers.
  * Implemented `FontManager::MeasureTextWrappedHeight()` to dynamically compute wrapped multi-line text height for descriptions.
  * Dynamically calculated vertical positioning for full armor set banners and right-hand inspection drawers across Helmets, Chestplates, and Leggings, preventing visual overlap across all screen resolutions.

### 3. Totem of Insanity Rework & Low-Health Blood Frenzy
* **High-Risk High-Reward Stat Matrix**:
  * Baseline +65% overall damage amplification across light, heavy, combo, and special attacks.
  * +35% movement speed bonus and +25% attack speed (cooldown reduction).
  * -50% defense penalty and x1.50 incoming damage multiplier from all enemy attacks.
* **Low-Health Blood Frenzy Surge**:
  * When player health drops below 40%, the Totem of Insanity unleashes Blood Frenzy, escalating the damage multiplier from +65% to +135% (+2.35x total damage).
  * Implemented dynamic `Player::GetTotemDamageMultiplier()` across all attack chains, heavy strikes, combo finishers, and weapon skills.
  * Enhanced visual and UI feedback with custom red-bordered trait descriptions in the equipment drawer.

### 4. Northern Crags Expansion Map & Troll Den
* **Grand Northern Overworld Expansion**:
  * Added a major new realm towards the north of Zyphden (`GameState::NORTH_ZYPHDEN` and `MapEnvironment::NORTH_ZYPHDEN`) matching Zyphden's massive scale (`2400 x 1500` bounds).
  * Features snow-dusted stone bluffs, frost pines, frozen highland crags, monolith pillars, and a dedicated campfire waypoint (`Highland Crags Bonfire`, Stage #6 on the Fast Travel Map).
  * Added seamless northern border transition connecting Zyphden's northern mountain pass (`{ 1380.0f, 120.0f }`) to the southern exit pass of the Northern Crags (`{ 1200.0f, 1450.0f }`) using the unified realm fade transition.
* **New Highland Mobs & Frost Troll Clan**:
  * Added Frost Troll enemies with heavy spiked war clubs, frostbite cleaves, and high composure resistance.
  * Strategic patrol packs combining Frost Trolls, Iron Brutes, and Corrupted Slashers across highland ridges.

### 5. Berserker Troll Boss Encounter, 25% Totem Drop Rate & Rematch System
* **Berserker Troll Boss**:
  * Giant horned highland boss wielding a massive spiked war club with devastating overhead smashes, ground-pound shockwaves, and rage sweeps.
  * Rebalanced Totem of Insanity to roll on an exact 25% drop chance upon defeat, accompanied by radiant loot sparks and sound effects.
  * If already owned or roll fails, awards an alternate trophy bounty (+100 Silver Shingles and +5 Ore Scraps).
* **Courtyard-Grade 30-Second Rematch System & Altar Cooldown UI**:
  * After defeating the Berserker Troll, players can re-engage the boss at the Blood Altar Dais (`{ 1200.0f, 240.0f }`) on a 30-second rematch cooldown timer.
  * Added persistent in-world floating recovery banner (`[ALTAR REMATCH: 24s]`) directly above the Blood Altar with real-time countdown updates.

### 6. Four New Highland Lore Fragments & Codex Expansion
* **Expanded Lore Chronicles (Fragments XI - XIV)**:
  * Increased `GameLimits::LORE_FRAGMENT_COUNT` from 10 to 14.
  * `frag_11`: Fragment XI: The Frigid Highland War (Ancient Outpost Monolith).
  * `frag_12`: Fragment XII: Curse of the Berserker (Troll Burial Mound).
  * `frag_13`: Fragment XIII: The Shattered Totem of Blood (Blood Altar Inscription).
  * `frag_14`: Fragment XIV: Watch of the Northern Vanguard (Vanguard Lookout Cairn).
  * Fully integrated into the Grimoire Lore Journal and Fast Travel stage overview.

### 7. Ore Golem Unbounded Pursuit in Royal Combat Arena
* **Arena AI Leash Removal**:
  * Added `isLeashed` property to `Enemy` entity structure.
  * Overworld Quarry Golems remain territorial and tethered to their ore veins, while Arena Ore Golems are completely unbound, relentlessly pursuing the player across the full arena floor without tethering, de-aggroing, or returning to spawn points.

### 8. Royal Combat Arena Expansion (Tier 5: Waves 21 - 25 & Apex Boss)
* **Tier 5: Frostbound Highland Trials**:
  * Expanded `WaveManager` total waves from 20 to 25.
  * Wave 21: Highland Glacial Recon (2 Frost Trolls, 2 Slashers, 2 Gale Bandits).
  * Wave 22: Basalt & Crags Crush (2 Frost Trolls, 2 Unleashed Ore Golems, 2 Archers).
  * Wave 23: Frostfire Mayhem (3 Frost Trolls, 2 Void Pyromancers, 2 Chargers).
  * Wave 24: Crag Warlord Vanguard (3 Frost Trolls, 2 Mini Elemental Golems, 2 Iron Brutes).
  * Wave 25: APEX FINAL BOSS ROUND 5 - The Berserker Troll Warlord (+2 Frost Trolls, +2 Pyromancers).
* **Milestone Rewards & Apex Victory**:
  * Milestone 4 (Wave 20 Cleared): +40 Ore Scraps, +120 Shingles, +4 Mist Essence, +3 Shadow Essence.
  * Grand Apex Victory (Wave 25 Cleared): +60 Ore Scraps, +200 Shingles, +6 Mist Essence, +5 Shadow Essence, and the Grand Apex Victor's Crest quest seal.

### 9. Humoral Bestiary Codex Expansion (Entries 10 & 11)
* **New Bestiary Cards**:
  * Increased `GameLimits::BESTIARY_COUNT` from 9 to 11.
  * Entry 10: **Frost Troll** (*Trollus Glacialis* - Melancholic Frost).
  * Entry 11: **Berserker Troll Warlord** (*Trollus Furens* - Sanguine Blood Boss).
  * Included complete Latin binomials, drops, combat styles, tactical weaknesses, and 3-tier unlockable lore seals (1, 5, and 15 kills).
  * Wired kill tracking across North Zyphden and Royal Combat Arena encounters.

### 10. Blood Frenzy Visual Feedback & Active Status Pill
* **Active Status Tray Indicator**:
  * Added `[BLOOD FRENZY: +135% DMG]` status pill in the action bar buff tray when health is below 40% with Totem of Insanity equipped.
* **Crimson Aura Particle Feedback**:
  * Player sprite renders a pulsing crimson aura gradient and warning rings during active Blood Frenzy.

### 11. Lore Journal Crash Fix & Dynamic Bookmark Layout
* **Out-of-Bounds Crash Resolution**:
  * Fixed a fatal crash (`SIGSEGV`) occurring upon opening the Grimoire Lore Journal (`Tab 1` / `[I]` key).
  * Root cause: fixed-size stack array `const char* romanNumerals[10]` in `ModalUI.cpp` was dereferenced out of bounds when `GameLimits::LORE_FRAGMENT_COUNT` was increased from 10 to 14.
  * Implemented `ToRomanNumeral(int value)` to dynamically convert fragment numbers into Roman numeral labels without fixed buffer limitations or hardcoded array bounds.
* **Dynamic Bookmark Layout & Flow Resizing**:
  * Replaced the hardcoded 2-row x 5-column grid with dynamic row and column calculations adapting cleanly to any fragment count (e.g. 5 columns for $\le 10$ fragments, 7 columns for 11-21 fragments).
  * Dynamically calculated bookmark tab widths (`tabW`), tab heights (`tabH`), and vertical spacing.
  * Dynamically positioned the active chapter summary header (`sumY`), recovery clue parchment plaque (`clueRec`), status badge (`badgeY`), and page turn buttons (`navBtnY`), eliminating visual overlap between bookmarks and parchment content regardless of fragment count.
  * Added defensive guards against empty fragment collections.
  * Updated lore progress counter in the Chapter Complete summary screen to dynamically query the inventory size rather than a hardcoded `/ 4`.

---

## Version 0.0.1g - Equipment Overhaul, Crusader Totem & Golem Synergy

### 1. Equipment Matrix & Tactical Inventory UI Overhaul
* **Reference-Grade Equipment Bubbles**:
  * Overhauled all armor slots (Helmet, Chestplate, Leggings) and the Totem Relic slot into modular equipment bubbles featuring precision corner brackets, tier pips, and glowing state borders.
  * Added dedicated category tags (`HEAD / HELMET`, `CHEST / CUIRASS`, `LEGS / GREAVES`, `TOTEM RELIC (ATTUNED PRIMAL CORE)`) with responsive selection and hover animations.
* **Vector Icons Across Matrix and Arsenal Drawers**:
  * Implemented custom vector-rendered icon art for all armor pieces across all six sets (Commoner's Hood/Tunic/Breeches, Crusader Helm/Cuirass/Greaves, Vanguard Greathelm/Chest/Greaves, Shadowstrider Cowl/Garb/Leggings, Scrap Sallet/Cuirass/Greaves, and Basalt Greathelm/Cuirass/Greaves).
  * Implemented custom vector icons for all five sacred totems (Totem of Sanity, Totem of Insanity, Totem of Mist, Crusader's Totem, and Elemental Core Totem).
  * Standardized drawer cards across all five inventory categories (Helmets, Chestplates, Leggings, Totems, and Weapons) with dedicated 38x38 icon boxes and clean typography.
* **Color-Coded Totem Buff & Debuff Indicators**:
  * Overhauled totem attribute descriptions into distinct color-coded segments: positive buffs rendered in luminous emerald/cyan (`[+] ...`), neutral separators in silver (` | `), and negative debuffs rendered in bold crimson red (`[-] ...`), resolving text legibility in both the matrix slot and drawer repertoire.
* **HUD Armor & Weapon Loadout Badges**:
  * Added mini modular loadout badges on the player HUD card for Vanguard Chest, Vanguard Greaves, Commoner's Tunic, and Commoner's Breeches with active set indicators.

### 2. Crusader's Totem Overhaul & "Will of Atlas" Crisis Passive
* **Rebalanced Relic Multipliers**:
  * +25% Physical Defense rating.
  * +35 Maximum Health pool increase.
  * +18% Overall Damage amplification.
  * +15 Willpower stat bonus.
  * -12% Movement Speed penalty.
  * -20% Attack Speed recovery penalty.
* **"Will of Atlas" Passive Invulnerability**:
  * Auto-activates whenever player health drops below 20% (or upon sustaining an incoming burst strike that would otherwise prove lethal).
  * Grants 4.0 seconds of complete invulnerability frames accompanied by rotating celestial runic circles, a divine golden aegis barrier, and floating status notifications.
  * Balances crisis survivability with an internal 60.0-second cooldown timer.

### 3. Mini Elemental Golem Companion Combat Overhaul
* **Empowered Base Resilience**:
  * Companion Health scaled from 105 HP to 380 HP.
  * Companion Defense scaled from 11 DEF to 42 DEF.
* **Three New Tactical Attack Patterns**:
  * **Heavy AoE Ground Slam** (engagement range < 85px, 3.5s cooldown):
    * Golem leaps upward and smashes into the earth, dealing 36 physical damage, triggering ground screen shake, releasing radial dust shockwaves, and staggering nearby hostiles.
  * **Forward Bash with Knockback** (engagement range 50-130px, 2.5s cooldown):
    * Golem rushes forward with immense kinetic impulse, dealing 28 physical damage and inflicting a 320.0f knockback surge that creates space for the player.
  * **Elemental Spike Throw** (engagement range 110-280px, 2.8s cooldown):
    * Golem hurls high-velocity crystallized earthen spikes dealing 26 damage with piercing trajectory through lines of enemies.
  * **Fallback Melee Punch** (engagement range < 45px, 1.0s cooldown):
    * Rapid punch dealing 18 damage between cooldown cycles.

### 4. Knight Lenon Courtyard Rematch & Crusader Totem Drop
* **Repeatable Courtyard Sparring**:
  * Added a persistent `[G] SPAR REMATCH (30s Cooldown)` interaction prompt beside Knight Lenon in the Zyphden Courtyard.
  * Preserves full access to the 20-wave Royal Combat Arena via `[R] ENTER ARENA` while allowing infinite sparring duels with Knight Lenon on a 30-second recovery cooldown. Fixed a despawn issue by ensuring Lenon remains persistently anchored in the courtyard after rematches.
* **25% Totem Drop Probability**:
  * Knight Lenon now rolls a 25% drop rate to award the Crusader's Totem upon defeat (active on both initial story victory and all subsequent courtyard rematches).
  * If the player already owns the Crusader's Totem, rematch victories award an alternate bounty of +80 Silver Shingles and +4 Ore Scraps.

### 5. Windrunner's Recurve Bow Balance Tuning
* **Raw Damage Calibration**:
  * Reduced base raw damage while preserving rapid draw cadence, low stamina consumption, and on-hit windburst AoE:
    * Tier 0 (Base): Light Attack 9.0 dmg, Heavy Attack 36.0 dmg.
    * Tier 1 (Refined): Light Attack 11.0 dmg, Heavy Attack 44.0 dmg.
    * Tier 2 (Masterwork): Light Attack 13.0 dmg, Heavy Attack 52.0 dmg.
  * Maintains on-hit 55px Windburst dealing 30% weapon damage and heavy Gale Piercer Drill with 50% defense pierce and armor fracture.

### 6. HUD Readability, Vitality Graduation Ticks & Screen Threat Vignette
* **Resource Bar Graduation Ticks & Metallic Glass Highlights**:
  * Added 25%, 50%, and 75% tactical graduation ticks to the Health, Stamina, and Poise bars for quick combat calculations.
  * Rendered sleek specular top-gradient shine highlights and dark metallic rounded borders.
* **Low-Health Threat Vignette**:
  * When player health falls below 25%, the screen dynamically pulses with a soft crimson edge vignette, warning the player and visually framing the impending "Will of Atlas" 20% threshold.
* **Boss Health Bar Polish & Stagger Meters**:
  * Added missing humoral affiliation badges for Gale Archer (`[PHLEGMATIC - WOODLAND GALE]`) and Elemental Golem (`[MELANCHOLIC - PRIMORDIAL BASALT]`).
  * Added a dedicated posture and stagger resistance guard bar beneath boss health bars.

### 7. Active Combat Buff & Relic Status Tray
* **Status Tray Above Action Bar**:
  * Positioned a row of illuminated status pills with real-time timers directly above action slots 1-6:
    * **Will of Atlas**: Radiant gold pill displaying remaining invulnerability (`[WILL OF ATLAS: 3.4s]`) or recovery cooldown (`[ATLAS CD: 45s]`).
    * **Aegis Shield**: Divine cyan pill tracking active barrier pool (`[AEGIS SHIELD: 60 HP]`).
    * **Smoke Mist Evasion**: Emerald pill showing +40% evasion bonus while inside smoke clouds (`[SMOKE MIST: +40% EVA]`).
    * **Phlegmatic Trance**: Vivid cyan glowing pill tracking active trance duration (`[PHLEGMATIC TRANCE: 5.2s]`).
    * **Counter Buff**: Solar gold pill tracking amplified damage window (`[COUNTER BUFF: +50% DMG]`).
    * **Bleeding**: Crimson pill displaying remaining debuff time (`[BLEEDING: 2.5s]`).

### 8. Combat Juice & Specialized Floating Combat Text
* **Thematic Damage Palettes**:
  * Replaced monochrome floating damage text with specialized damage palettes:
    * Standard Physical: Clean Silver-White (`Color{ 240, 245, 255, 255 }`).
    * Critical Strikes: Radiant Gold (`Color{ 255, 220, 60, 255 }`, 24px scale).
    * Solar Burn DoT: Solar Amber (`Color{ 255, 195, 50, 255 }`).
    * Poison DoT: Toxic Emerald (`Color{ 90, 245, 140, 255 }`).
    * Bleed DoT: Arterial Crimson (`Color{ 245, 60, 75, 255 }`).
* **Tactical Deflection Popups**:
  * Added distinct popups for `PERFECT PARRY!`, `HYPER-ARMOR DEFLECT!`, and `STAGGERED!`.

### 9. Multi-Slot Equipment Loadout Presets & Live Drawer Stat Deltas
* **Functional Loadouts [ I ] [ II ] [ III ]**:
  * Enabled 3 quick-swap loadout presets in the Equipment Matrix, instantly equipping saved weapon, helmet, chestplate, leggings, and totem combinations.
  * Added a dedicated `[SAVE]` snapshot button to manually update the active loadout preset.
* **Drawer Stat Comparison Deltas**:
  * Added live relative comparison deltas when inspecting unequipped helmets, chestplates, leggings, and weapons in the drawer, displaying differences in defense, willpower, max HP, attack damage, reach, and armor penetration in emerald green or soft red.

### 10. Crucible Rematch Encounters & World Cooldown Timers
* **Knight Lenon World Cooldown Banner**:
  * Added a floating resting cooldown banner (`[RESTING (Xs)]`) above Knight Lenon in the Zyphden Courtyard during his 30-second recovery period.
* **High Altar Rematch Crucible (Ruined Castle)**:
  * Implemented an interactive prayer pedestal and sacred aura circle at the Cathedral High Altar, allowing players to spar with Zil's Remnant on a 30-second cooldown for +75 Silver Shingle rewards.

---

## Version 0.0.1f - Arena Milestones, Soundscapes & Combat Polish

### 1. Royal Combat Arena Wave Milestones & Rewards
* **Milestone Resource Chest Payouts**:
  * **Wave 5 Milestone**: 10 Silver Shingles and 4 Ore Scraps with floating HUD reward banner.
  * **Wave 10 Milestone**: 20 Silver Shingles, 8 Ore Scraps, and 1 Essence of Mist.
  * **Wave 15 Milestone**: 30 Silver Shingles, 12 Ore Scraps, 2 Essence of Mist, and 1 Shadow Essence.
  * **Wave 20 Grand Victory**: 50 Silver Shingles, 20 Ore Scraps, 4 Shadow Essence, and unlocks the **Victor's Crest** quest trophy.
* **Dynamic Arena HUD**: Real-time wave counter, boss milestone markers, and victory celebration banner.

### 2. Procedural Audio Ambience & Dynamic Boss Combat Ducking
* **Procedural Soundscapes in SoundFX.cpp**:
  * `PlayAnvilStrike()`: Dynamic anvil clink with high-frequency iron resonance near Blacksmith Varek in Zyphden.
  * `PlayHearthCrackle()`: Warm crackling ember sounds inside the Zyphden Tavern near the central hearth.
  * Ambient wind rushing through trees in the Restless Mist Forest.
* **Dynamic Boss Combat Ducking (`SoundFX::SetBossCombatMode(bool)`)**:
  * Background audio smoothly attenuates down to 38% volume during intense boss duels (Knight Lenon, Gale Archer, Zil, and Arena boss waves 5, 10, 15, and 20).
  * Audio smoothly fades back to full ambient volume upon boss defeat or combat exit.

### 3. Combat Feedback & Poise Tuning
* **Heavy Weapon Hyper-Armor**:
  * Heavy attack windup on colossal armaments (Sun Claymore, Mace, Halberd, Storm Glaive, and Vanguard Bastion set) grants complete stagger interruption immunity.
  * Incoming damage mitigated by 30% during hyper-armor with golden deflection sparks and parry audio cues.
* **Basalt Golem Armor Poise Feedback**:
  * Full 3/3 Basalt Golem set grants 70% knockback resistance against hostile attacks.
  * Absorbing heavy strikes triggers grounded screen shake (3.5f) and dense stone dust impact particles.
* **Standardized Projectile Collision Hitboxes**:
  * Added swept-circle line collision testing in `Projectile.cpp` for elongated directional projectiles, eliminating tunneling and ghost misses.

### 4. Totem of Mist Companion Revamp & Dynamic Tracking
* Rebalanced Totem of Mist: removed damage output penalty, replaced with -15% defense multiplier and -25 Max Health penalty.
* Summons an ethereal Mist Fairy companion that dynamically follows the player with smooth interpolation and leash snapping (>260px distance).
* Mist Fairy grants +20% Critical Chance and +35% Critical Damage, periodically generating restorative healing orbs every 14 seconds (+30 HP on contact).

### 5. Windrunner's Recurve Bow Rebalance & Impact Windburst
* Light attack arrows no longer pierce enemies; instead, upon impact, arrows trigger a Windburst AoE (55px radius) dealing 30% weapon damage to nearby enemies.
* Heavy attack fires Gale Piercer Drill Arrow with drawn bowstring charging visuals, concentric wind rings, 50% defense pierce, on-hit Armor Fracture (-40% enemy defense for 4s), and 1.2s stun/stagger.

### 6. Armor Weapon Synergies & Vanguard Bastion Set
* Added tailored weapon synergies for each armor set:
  * Commoner's Scrap: +20% Stamina Regen & +15% Loot Find; Sword synergy grants +15% light attack speed and -25% stamina cost.
  * Crusader Plate: +20% Defense & +25 Will; Truth Seeker & Broadsword synergy grants +25% holy damage and +20% beam defense pierce.
  * Vanguard Bastion: +50 Max HP & unbreakable hyper-armor during attacks; Halberd & Glaive synergy extends weapon reach +35px with +25% armor penetration and concussive shockwaves. Special skill [V]: Phalanx Vanguard Charge (rushes 180px, deals 65 damage, -30% armor shred for 5s, 1.8s stun, and 50 HP Fortification shield).
  * Shadowstrider: +20% Move Speed, -30% Dash Stamina Cost, +0.10s Dodge iframes; Daggers & Bow synergy grants +15% Crit Chance. Perfect evades roll a 15% chance to summon an AoE Smoke Mist (95px radius, 7s duration with automatic cleanup) granting 40% evasion while inside and inflicting 14 poison DoT every 0.45s to enemies.
  * Forged Scrap Iron: +18 Defense, +20% Poise Damage; War Mace synergy inflicts -25% Armor Fracture and grants +50% extra composure on hit.
  * Basalt Golem: +35 Defense, 70% knockback resistance; Sun Claymore synergy unleashes a secondary molten shockwave (+30 earth damage) with unbreakable hyper-armor.

---

## Version 0.0.1e - Master Forge, Heavy Arsenal & Subterranean Depths

### 1. Master Forge Weapon Pagination & High-Contrast Typography
* Divided Vanguard Weapons in Blacksmith Varek's Master Forge into a 2x2 paginated grid (4 cards per page across 2 pages).
* Implemented `< PREV PAGE` and `NEXT PAGE >` navigation with dual mouse and keyboard shortcuts (`A`/`D` and Arrow keys).
* Rescaled typography across headers, tabs, item titles, damage statistics, and upgrade requirements from 9-11pt to 12-16pt with high-contrast palette values.

### 2. Weapon Arsenal Expansion
* Added `WeaponType::SUN_CLAYMORE` and `WeaponType::STORM_GLAIVE`:
  * **Sun-Forged Claymore**: Light attack: 34 base dmg (sweeping 120-degree arc); Heavy attack (*Solar Rupture*): 92 dmg with expanding holy shockwave ring and hyper-armor damage mitigation.
  * **Storm Glaive**: Light attack: 28 base dmg (extended linear spear thrust); Heavy attack (*Tempest Vortex*): 78 dmg with a 240-degree sweeping cleave that inflicts high poise knockback.
* Rebalanced `WeaponType::HALBERD` heavy combo damage scaling from 28/34/40 to 58/72/88 across upgrade tiers.

### 3. Material Economy Rebalance
* Reduced common mob Shingles drop rates from guaranteed 2-5 down to 1-3 Shingles at a 60% probability.
* Subterranean Ore Golems drop 1 Ore Scrap at a 65% chance with respawn timer increased from 7.0s to 18.0s.

### 4. Save Slot Persistence Isolation & Companion Warping
* Reset all 5 realm handlers prior to state restoration on New Game or slot load to prevent cross-slot data contamination.
* Added `elementalGolemDefeated` to save metadata summaries.
* Added `Player::RelocateCompanionToPlayer()` and 350px leash warping safeguard to prevent companions from getting lost across transitions.

### 5. Architectural Restructuring & Dedicated UI Subsystem
* Bundled interface renderers and modals (HUD, ModalUI, TitleUI, UI) into `src/ui/`.
* Separated helper parsers and limits into `src/core/GameConstants.hpp` and `SaveHelpers.hpp`.

---

## Version 0.0.1d - Input Architecture, Royal Halberd & Tactical HUD

### 1. Modular InputRouter & Centralized Hotkeys
* Decoupled key event polling from rendering passes, eliminating dropped input frames during single-frame buffer consumption.
* Centralized modal navigation in `Game::Update` for seamless toggling of Map (`[M]`), Gear (`[I]`/`[TAB]`), Controls (`[C]`), and Settings.

### 2. Royal Halberd 3-Stage Heavy Attack Combo
* Stage 1: Long Lunge with i-frames (forward surge with 0.22s invulnerability).
* Stage 2: Wide Sweeping Cleave (220-degree horizontal polearm cleave).
* Stage 3: Flurry of 3 rapid linear thrusts dealing 30% base weapon damage per strike.

### 3. Tactical Gear & Inventory Overhaul
* Symmetrical 3-column tactical layout with interactive drawer cards and paper doll showcase.
* Added live material counters, weapon quick-cycle tabs, and set bonus preview cards.

### 4. In-Game Procedural Audio Mixer
* Real-time sliders inside settings (`[C]` -> Audio Mixer) for Master Gain, SFX Volume, and Ambient Soundscapes.

---

## Version 0.0.1c - Interactive Realm Cartography & Quarry Mines

### 1. Interactive Realm Cartographic Guild Survey (`[M]`)
* Guild survey map featuring pan/zoom navigation, recenter shortcuts (`[SPACE]`/`[R]`), live player coordinate tracking beacon, and 8 Points of Interest with category filter toggles.

### 2. Subterranean Quarry Depths & Elemental Golem Boss
* Gated eastern archway in Zyphden Courtyard unlocking upon defeating Knight Lenon.
* Four basalt Ore Golem spawners dropping Ore Scraps for forge crafting.
* Primordial Altar requiring 10 Lore Fragments (`frag_01` through `frag_10`) to summon the Elemental Golem titan (850 HP, 55 DEF, 90 Will).
* Reward: Elemental Core Totem granting flat defense and light attack projectile attacks.

### 3. Decoupled Screen-Space Modal Architecture
* Removed all rendering calls from `Game::Update`, eliminating screen freezing when opening modals.
* Unified screen-space modal overlay executed safely between `BeginDrawing()` and `EndDrawing()`.

---

## Version 0.0.1b - Typography, Multi-Slot Persistence & Economy

### 1. Medieval Typography Engine
* Custom `FontManager` with custom glyph mappings, special character sanitization, and graceful ASCII fallbacks across all UI states and HUD elements.

### 2. Multi-Slot Persistence Architecture
* Independent 3-slot save system (`save_slot_1.json`, `save_slot_2.json`, `save_slot_3.json`) with automated background state saving and title screen slot inspection.

### 3. Silver Shingles Economy & Physical Loot Drops
* Physical ground coin drops with arc and bounce trajectory physics, vacuum magnet attraction toward player, and HUD currency counters.

### 4. Blacksmith Varek & Hearthside Tavern
* Blacksmith Varek offering weapon crafting and upgrade tiers (Base, +1 Refined, +2 Masterwork, Consecrated Relic).
* Tavernkeep Eldon offering consumable provisions inside the Zyphden settlement.

### 5. Phlegmatic Composure & Four Humors System
* Composure gauge (0-100), passive composure tiers (Discomposed, Composed, Stoic Poise), and Phlegmatic Trance active invulnerability aura (`[SPACE]`).
* Four Humoral alignments (Sanguine, Choleric, Melancholic, Phlegmatic) with Quenching and Shattering mechanics.

