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
};

void sprite_path_none_update_position(struct Sprite *sprite)
{
}

void sprite_path_1_update_position(struct Sprite *sprite)
{
    sprite->precision_world_xpos += (1<<16)/2;
}

void sprite_path_2_update_position(struct Sprite *sprite)
{
    uint16_t sin_cos_index;

    if (sprite->frames_alive >= 64 && sprite->frames_alive < (64+256)) {
        sin_cos_index = (sprite->frames_alive - 64);
        sprite->precision_world_xpos += cos_table[sin_cos_index];
        sprite->precision_world_ypos += sin_table[sin_cos_index];
    } else {
        sprite->precision_world_xpos += (1<<16)/2;
    }
}
