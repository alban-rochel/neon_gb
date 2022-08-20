#include "gb/gb.h"

#include "neon_level.h"
#include "sprite.h"


// The metasprite will be built starting with hardware sprite zero (the first)
#define SPR_NUM_START 0

// Metasprite tiles are loaded into VRAM starting at tile number 0 
#define TILE_NUM_START 0

#define STATE_IDLE 0
#define STATE_RUNNING 1
#define STATE_JUMPING 2
#define STATE_FALLING 3
#define STATE_MASK 0x07
// 3 bits

#define STATE_MOD_LOOKING_LEFT 0x08
#define STATE_MOD_ATTACKING 0x10

#define ACCEL_X 2
#define ACCEL_Y 4
#define BRAKE_Y 1
#define MAX_VELOCITY_X 32
#define MAX_VELOCITY_Y 32
#define POSITION_SHIFT 4

uint8_t current_state;
uint8_t frame_count;
uint8_t char_change;

uint16_t world_pos_x;
uint16_t world_pos_y;
int16_t speed_x;
int16_t speed_y;
uint8_t jmp_count;
uint8_t falling;
uint8_t left_moving;
int16_t viewport_x_target;
int16_t current_viewport_x;

const metasprite_t* current_sprite;

void update_character()
{
    uint8_t joy = joypad();

    if(joy & J_LEFT)
    {
        if(speed_x > 0)
            speed_x = 0;

        left_moving = 1;
        
        speed_x -= ACCEL_X;
        if(speed_x < - MAX_VELOCITY_X)
        {
            speed_x = - MAX_VELOCITY_X;
        }
    }
    else if(joy & J_RIGHT)
    {
        if(speed_x < 0)
            speed_x = 0;

        left_moving = 0;

        speed_x += ACCEL_X;
        if(speed_x > MAX_VELOCITY_X)
        {
            speed_x = MAX_VELOCITY_X;
        }
    }
    else
    {
        speed_x = 0;
    }

    world_pos_x += speed_x;

    if((joy & J_A) && (falling == 0))
    {
        speed_y -= ACCEL_Y;
        if(speed_y < - MAX_VELOCITY_Y)
        {
            speed_y = - MAX_VELOCITY_Y;
        }
        ++ jmp_count;
        if(jmp_count > 15)
        {
            falling = 1;
        }
    }
    else
    {
        speed_y += 2;
        if(speed_y > MAX_VELOCITY_Y)
        {
            speed_y = MAX_VELOCITY_Y;
        }
    }

    world_pos_y += speed_y;

    uint8_t ground_level_hack = 144;
    {
        uint8_t pos_x_hack = (world_pos_x >> POSITION_SHIFT);
        if(pos_x_hack >= 84 && pos_x_hack < 140)
        {
            ground_level_hack = 112;
        }
        else if(pos_x_hack >= 140 && pos_x_hack < 252)
        {
            ground_level_hack = 128;
        }
    }



    if(world_pos_y >= (ground_level_hack << POSITION_SHIFT))
    {
        world_pos_y = (ground_level_hack << POSITION_SHIFT);
        falling = 0;
        jmp_count = 0;
        speed_y = 0;
    }

    if(speed_y < 0)
    {
        current_sprite = sprite_metasprites[5]; // jump
    }
    else if(speed_y > 0)
    {
        current_sprite = sprite_metasprites[6]; // fall
    }
    else
    {
        if(speed_x == 0)
        {
            uint8_t temp = frame_count >> 5;
            if(temp & 0x01)
            {
                current_sprite = sprite_metasprites[0];
            }
            else
            {
                current_sprite = sprite_metasprites[1];
            }
        }
        else
        {
            uint8_t temp = frame_count >> 3;
            if(temp & 0x01)
            {
                current_sprite = sprite_metasprites[3];
            }
            else
            {
                if(temp & 0x02)
                {
                    current_sprite = sprite_metasprites[4];
                }
                else
                {
                    current_sprite = sprite_metasprites[2];
                }
            }
        }
    }

    if(left_moving)
    {
        //viewport_x_target = (world_pos_x >> POSITION_SHIFT) - 160;
    }
    else
    {
        viewport_x_target = (world_pos_x >> POSITION_SHIFT) + 200;
    }

    current_viewport_x += (viewport_x_target - current_viewport_x) >> 4;
    move_bkg(current_viewport_x, 8);

    if(left_moving)
    {
        uint8_t pos_x = (world_pos_x >> POSITION_SHIFT) - current_viewport_x;
        if(pos_x > 200)
        {
            pos_x = 0;
        }
        move_metasprite_vflip       (current_sprite, TILE_NUM_START, SPR_NUM_START, pos_x/*(world_pos_x >> POSITION_SHIFT) - current_viewport_x*/, world_pos_y >> POSITION_SHIFT);
    }
    else
    {
        move_metasprite       (current_sprite, TILE_NUM_START, SPR_NUM_START, (world_pos_x >> POSITION_SHIFT) - current_viewport_x, world_pos_y >> POSITION_SHIFT);
    }
}

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

    frame_count = 0;

    current_state = STATE_IDLE; // Looking right, not attacking

    move_metasprite(sprite_metasprites[0], TILE_NUM_START, SPR_NUM_START, 100, 100);

    world_pos_x = 24 << POSITION_SHIFT;
    world_pos_y = 144 << POSITION_SHIFT;
    speed_x = 0;
    speed_y = 0;
    falling = 0;
    current_sprite = sprite_metasprites[0];
    left_moving = 0;
    current_viewport_x = 200;

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
        /*if(frame_count == 40)
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
        }*/

        update_character();

        ++frame_count;
        wait_vbl_done();  
    }
}