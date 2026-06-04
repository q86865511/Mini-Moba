# MOBA Interaction Asset Pack

Interaction-ready prototype assets for a MOBA-style game.

## Contents

- 5 hero sprite sheets with idle/move/attack/hurt/death states.
- 6 lane minion sprite sheets: blue/red melee, ranged, and siege.
- 3 neutral monster sprite sheets.
- 2 tower sprite sheets and 2 main core sprite sheets.
- Map interaction zones for lanes, towers, bases, brush, and jungle camps.
- Map prop sheet for brush, camp markers, river rune, lane waypoint, vision ward, and healing orb.

## Runtime Notes

- Each sprite sheet uses 256x256 frames and 6 columns.
- Every `*_actions.json` file includes frame rectangles, anchors, collision circles, hurtboxes, and attack events.
- Use `map/interactive_map_zones.json` for prototype navigation, aggro, brush stealth, jungle respawn, and win-condition wiring.

## Preview

- `outputs/moba_interaction_pack/preview/moba_interaction_pack_overview.png`
