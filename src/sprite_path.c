#include <stddef.h>
#include "sprite_path.h"
#include "sprite_system.h"

struct SpritePath sprite_paths[] = {
    // type 0 - none
    {
        &sprite_path_none_update_position
    },
    // type 1 - slow left movement
    {
        &sprite_path_1_update_position
    },
};

void sprite_path_none_update_position(struct Sprite *sprite)
{
}

void sprite_path_1_update_position(struct Sprite *sprite)
{
    sprite->precision_world_xpos += (1<<16)/2;
}

