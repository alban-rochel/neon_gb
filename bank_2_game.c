#include "gb/gb.h"

#include "neon_level.h"
#include "sprite.h"


// The metasprite will be built starting with hardware sprite zero (the first)
#define SPR_NUM_START 0

// Metasprite tiles are loaded into VRAM starting at tile number 0 
#define TILE_NUM_START 0

void game_loop() BANKED
{
    SPRITES_8x16;
    BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);

    set_bkg_data(0x0, neon_level_tile_count, neon_level_tile_data);
    set_bkg_tiles(0, 0, neon_level_tile_map_width, neon_level_tile_map_height, neon_level_map_data);

    // Load metasprite tile data into VRAM
    set_sprite_data(TILE_NUM_START, sizeof(sprite_tiles) >> 4, sprite_tiles);
    SCX_REG = 0;
    SCY_REG = 0;

    // show bkg and sprites
    SHOW_BKG; SHOW_SPRITES;

    uint8_t frame_count = 0;

    while(1)
    {
        /*if((frame_count >> 5) & 0x1)
        {
            move_metasprite       (sprite_metasprites[0], TILE_NUM_START, SPR_NUM_START, 100, 100);
        }
        else
        {
            move_metasprite       (sprite_metasprites[1], TILE_NUM_START, SPR_NUM_START, 100, 100);
        }*/
        if(frame_count == 40)
        {
            move_metasprite       (sprite_metasprites[2], TILE_NUM_START, SPR_NUM_START, 100, 100);
            frame_count = 0;
        }
        else if(frame_count == 10 || frame_count == 30)
        {
            move_metasprite       (sprite_metasprites[3], TILE_NUM_START, SPR_NUM_START, 100, 100);
        }
        else if(frame_count == 20)
        {
            move_metasprite       (sprite_metasprites[4], TILE_NUM_START, SPR_NUM_START, 100, 100);
        }
        ++frame_count;
        wait_vbl_done();  
    }
}