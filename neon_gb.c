#include <gb/gb.h>
//#include <gbdk/console.h>

// bank 1 (title)

void title_loop() BANKED;

// bank 2 (game)

void game_loop() BANKED;

void main() {
/*	BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_LITE_GRAY, DMG_WHITE);
// First load the tile patterns in the Background Tile Pattern table. This
// means that we load all the 8x8 images that make up our picture (131 of them
// to be exact).
    set_bkg_data(0, neon_gb_tile_count, neon_gb_tile_data);

    VBK_REG = 1;
    VBK_REG = 0;

// Then we set the tile pattern in the background tile table. The variable
// `tilemap` contains an array of tile numbers that reference to the the tiles
// we loaded previously using `set_bkg_data`.
//
// Our image is `20` tiles wide and `18` tiles high, thus having a total of 360
// tiles. How is that possible when we only loaded `131` tiles?
//
// Well, that is because some in the image are identical. So instead the tile
// is only saved once in the `tiledata` variable and reused multiple times in
// the `tilemap` variable.
    set_bkg_tiles(0, 0, neon_gb_tile_map_width, neon_gb_tile_map_height, neon_gb_map_data);
// In order for the background to actually show up we call `SHOW_BKG`.
    SHOW_BKG;

    DISPLAY_ON;*/

	while(1)
	{
		SWITCH_ROM_MBC1(1);
		title_loop();

		SWITCH_ROM_MBC1(2);
		game_loop();
	}

}