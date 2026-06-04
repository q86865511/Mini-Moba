# MOBA Map Tiles

These files split the original `2048x2048` MOBA map into reusable PNG tiles.

## Recommended Use

- Use `tiles_256/` for simple loading, streaming, or hand-placing individual map chunks.
- Use the padded atlas when your engine samples from a texture atlas; each tile has a 2px edge extension around it.
- Use `*_tilemap.json` to rebuild the full map in row-major order.

## Generated Sets

### 256px tiles

- Tile count: `64` (`8 x 8`)
- Tile folder: `outputs/moba_asset_pack/map_tiles/tiles_256`
- Manifest: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_256_manifest.json`
- Tilemap JSON: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_256_tilemap.json`
- Atlas: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_256_atlas.png`
- Padded atlas: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_256_atlas_padded_2px.png`
- Grid preview: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_256_grid_preview.png`
- Pixel-perfect stitched check: `True`

### 128px tiles

- Tile count: `256` (`16 x 16`)
- Tile folder: `outputs/moba_asset_pack/map_tiles/tiles_128`
- Manifest: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_128_manifest.json`
- Tilemap JSON: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_128_tilemap.json`
- Atlas: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_128_atlas.png`
- Padded atlas: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_128_atlas_padded_2px.png`
- Grid preview: `outputs/moba_asset_pack/map_tiles/moba_map_tiles_128_grid_preview.png`
- Pixel-perfect stitched check: `True`
