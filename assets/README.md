# Custom Assets Directory

Drop your custom sprites and audio files here to integrate them directly into **Phlegmatic**.

The engine's `AssetManager` (`src/AssetManager.cpp`) automatically detects and binds custom assets placed here:

| Filename | Purpose | Recommended Dimensions | Notes |
| :--- | :--- | :--- | :--- |
| `player.png` | Player character sprite / sheet | 64x64 or 128x128 px | If omitted, procedural sleek silhouette is used |
| `enemy_melee.png` | Slasher enemy sprite | 64x64 px | Red-trimmed agile warrior |
| `enemy_brute.png` | Brute crusher boss sprite | 96x96 or 128x128 px | Heavy armored titan |
| `tileset.png` | Arena dungeon floor tiles | 64x64 px tiles | Auto-replaces stone slab procedural grid |
| `zyphden_transition.png` | Zyphden Realm Transition screen artwork | 1920x1080 (16:9) or 1280x720 px | Replaces the in-game transition placeholder screen |

Procedural drawing fallbacks are active by default, ensuring the game runs smoothly even without image files!
