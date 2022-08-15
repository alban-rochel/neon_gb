#include <gb/gb.h>
#include <gbdk/console.h>

#include "neon_title.h"

#define SCROLL_START_POSITION 150
#define SCROLL_END_POSITION 0

uint8_t scroller_pos = SCROLL_START_POSITION;

void title_loop() BANKED
{
    wait_vbl_done();

    scroller_pos = SCROLL_START_POSITION;

    set_bkg_data(0x0, neon_title_tile_count, neon_title_tile_data);

    VBK_REG = 1;
    VBK_REG = 0;

    set_bkg_tiles(0, 0, neon_title_tile_map_width, neon_title_tile_map_height, neon_title_map_data);

    SHOW_BKG;

    DISPLAY_ON;

    //BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_WHITE, DMG_LITE_GRAY, DMG_BLACK);

    while(scroller_pos != SCROLL_END_POSITION)
    {
        ++scroller_pos;
        SCY_REG = scroller_pos;

        if(joypad() & J_START)
        {
            return;
        }

        wait_vbl_done();
    }

    uint8_t frame_count = 0;

    while(1)
    {
        if(frame_count == 0 || frame_count == 128)
        {
            BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_WHITE, DMG_LITE_GRAY, DMG_BLACK);
        }
        else if(frame_count == 30 || frame_count == 158)
        {
            BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_WHITE, DMG_LITE_GRAY, DMG_BLACK);
        }

        ++frame_count;

        if(joypad() & J_START)
        {
            return;
        }

        wait_vbl_done();
    }
}