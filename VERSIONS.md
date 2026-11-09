# PHLEGMATIC - Version History & Patch Notes Compendium

*The complete changelog, system updates, combat balance revisions, and interface overhauls across development milestones for **Phlegmatic**.*

---

## Table of Versions

- [Version 0.0.1j - Combat Executions, Alchemy Workshop, Phlegmatic Core & Caverns (In-Development)](#version-001j---combat-executions-alchemy-workshop-phlegmatic-core--caverns-in-development)
- [Version 0.0.1i - Northern Arsenal, Highland Atmosphere, Outcast Bounties & Apex Boss Gauntlet](#version-001i---northern-arsenal-highland-atmosphere-outcast-bounties--apex-boss-gauntlet)
- [Version 0.0.1h - Northern Crags Expansion, Berserker Troll Boss & Totem of Insanity](#version-001h---northern-crags-expansion-berserker-troll-boss--totem-of-insanity)
- [Version 0.0.1g - Equipment Overhaul, Crusader Totem & Golem Synergy](#version-001g---equipment-overhaul-crusader-totem--golem-synergy)
- [Version 0.0.1f - Arena Milestones, Soundscapes & Combat Polish](#version-001f---arena-milestones-soundscapes--combat-polish)
- [Version 0.0.1e - Master Forge, Heavy Arsenal & Subterranean Depths](#version-001e---master-forge-heavy-arsenal--subterranean-depths)
- [Version 0.0.1d - Input Architecture, Royal Halberd & Tactical HUD](#version-001d---input-architecture-royal-halberd--tactical-hud)
- [Version 0.0.1c - Interactive Realm Cartography & Quarry Mines](#version-001c---interactive-realm-cartography--quarry-mines)
- [Version 0.0.1b - Typography, Multi-Slot Persistence & Economy](#version-001b---typography-multi-slot-persistence--economy)

---

## Version 0.0.1j - Combat Executions, Alchemy Workshop, Phlegmatic Core & Caverns (In-Development)

### In-Development Roadmap
1. **Visceral Execution & Stagger Deathblow System**: Poise-break vulnerability, execution reticles, melee visceral deathblows with i-frames and weapon-scaled critical burst.
2. **Alchemy Workshop & Quick-Item Belt**: Tavern Keeper Brena brewing station, combat tonics (Stoneskin, Glacial, Berserker, Swiftfoot), and 1-3 quick belt hotkeys.
3. **Totem of the Phlegmatic Core**: 7th sacred relic with Phlegmatic Apathy, gray health recovery, and counter-freeze aura.
4. **Dynamic Time-of-Day Lighting Cycle & Nightfall Incursions**: Day/Dusk/Night ambient transitions, point-light radiuses, and nocturnal elite spawns.
5. **Tactical Corner Mini-Map Radar & Compass HUD**: Top-right mini-radar with player heading, POIs, NPC markers, and enemy pips.
6. **Bestiary Mastery Hunting Perks**: Passive combat traits unlocked by Tier 2 and Tier 3 kill seals in the Bestiary Codex.
7. **Permafrost Crypts Subterranean Caverns**: Highland cave dungeon with icy platforms, breakable stalagmites, Frost Revenant boss, and Lore Fragment XV.
8. **Storage Stash Chest & Persistence**: Campfire and town stash chest for materials and loadouts with save serialization.
9. **File Architecture Restructuring & Subsystem Decoupling**: Extracting realm handlers into `src/states/realms/` and item systems into `src/items/`.

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

