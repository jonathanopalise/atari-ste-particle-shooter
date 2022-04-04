#include <stddef.h>
#include <inttypes.h>
#include "sprite_path.h"
#include "sprite_system.h"
#include "sin_cos.h"

struct SpritePath sprite_paths[] = {
    // type 0 - none
    {
        &sprite_path_none_update_position
    },
    // type 1 - slow left movement
    {
        &sprite_path_1_update_position
    },
    // type 2 - loop the loop from bottom right
    {
        &sprite_path_2_update_position
    },
    // type 3 - type 2 vertical mirror
    {
        &sprite_path_3_update_position
    },
    // type 4 - rotate in circles
    {
        &sprite_path_4_update_position
    },
};

void sprite_path_none_update_position(struct Sprite *sprite)
{
}

void sprite_path_1_update_position(struct Sprite *sprite)
{
    sprite->precision_world_xpos -= (1<<16);
}

void sprite_path_2_update_position(struct Sprite *sprite)
{
    uint16_t sin_cos_index;

    // need this
    sprite->precision_world_xpos += (1<<16);

    if (sprite->frames_alive >= 96 && sprite->frames_alive < (96+128)) {
        // this will give us a number in the range of 0-127
        sin_cos_index = (sprite->frames_alive - 96) << 1;
        sprite->precision_world_xpos -= cos_table[sin_cos_index] << 1;
        sprite->precision_world_ypos += sin_table[sin_cos_index] << 1;
    } else {
        sprite->precision_world_xpos -= cos_table[0] << 1;
        sprite->precision_world_ypos += sin_table[0] << 1;
    }
}

void sprite_path_3_update_position(struct Sprite *sprite)
{
    uint16_t sin_cos_index;

    // need this
    sprite->precision_world_xpos += (1<<16);

    if (sprite->frames_alive >= 96 && sprite->frames_alive < (96+128)) {
        sin_cos_index = (sprite->frames_alive - 96) << 1;
        sprite->precision_world_xpos -= cos_table[sin_cos_index] << 1;
        sprite->precision_world_ypos -= sin_table[sin_cos_index] << 1;
    } else {
        sprite->precision_world_xpos -= cos_table[0] << 1;
        sprite->precision_world_ypos -= sin_table[0] << 1;
    }
}

void sprite_path_4_update_position(struct Sprite *sprite)
{
    uint16_t sin_cos_index;

    // need this
    sprite->precision_world_xpos += (1<<16);

    sin_cos_index = (sprite->frames_alive & 63) << 2;
    //sprite->precision_world_xpos -= cos_table[sin_cos_index];
    //sprite->precision_world_ypos -= sin_table[sin_cos_index];
}
