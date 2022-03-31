#include "sprite_system.h"
#include "particle_system.h"
#include "random.h"

void mine_sprite_init_attributes(struct Sprite *sprite)
{
    sprite->image_index = SPRITE_IMAGE_MINE;
}

void mine_sprite_update_attributes(struct Sprite *sprite)
{
    sprite->precision_world_xpos += (1<<16)/2;
}

void mine_sprite_handle_player_bullet(struct Sprite *sprite)
{
    uint16_t particle_xpos;
    uint16_t particle_ypos;
    uint16_t x_index;
    uint16_t y_index;

    particle_ypos = sprite->precision_world_ypos;
    for (y_index = 0; y_index < 5; y_index++) {
        particle_xpos = sprite->precision_world_xpos;
        for (x_index = 0; x_index < 5; x_index++) {
            /*particle_system_spawn(
                particle_xpos,
                particle_ypos,
                (random() % 48000) - 24000,
                -(random() % 24000),
                PARTICLE_TYPE_PLAYER_EXHAUST
            );*/

        particle_system_spawn(
            sprite->precision_world_xpos,
            sprite->precision_world_ypos + (6 << 16) + random() % (4 << 16),
            (random() % 80000),
            -(random() % 80000),
            PARTICLE_TYPE_GENERIC
        );


            particle_xpos += (4 << 16);
        }
        particle_ypos += (4 << 16);
    }

    sprite->active = 0;
    // need to convert to explosion
}
