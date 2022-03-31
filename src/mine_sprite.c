#include "inttypes.h"
#include "sprite_system.h"
#include "particle_system.h"
#include "random.h"
#include "explosion_sprite.h"

#define EXPLOSION_SPAWN_PARAMS_COUNT 128

uint16_t current_explosion_spawn_params_offset;

struct ExplosionSpawnParams {
    int32_t precision_world_xoffset;
    int32_t precision_world_yoffset;
    int32_t precision_world_xadd;
    int32_t precision_world_yadd;
};

struct ExplosionSpawnParams explosion_spawn_params[EXPLOSION_SPAWN_PARAMS_COUNT];

void mine_sprite_init_type()
{
    for (uint16_t index = 0; index < EXPLOSION_SPAWN_PARAMS_COUNT; index++) {
        explosion_spawn_params[index].precision_world_xoffset = (6 << 16) + random() % (4 << 16);
        explosion_spawn_params[index].precision_world_yoffset = (6 << 16) + random() % (4 << 16);
        explosion_spawn_params[index].precision_world_xadd = (random() % 80000);
        explosion_spawn_params[index].precision_world_yadd = -(random() % 80000);
    }

    current_explosion_spawn_params_offset = 0;
}

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
    struct ExplosionSpawnParams *current_explosion_spawn_params;

    for (uint16_t index = 0; index < 16; index++) {
        current_explosion_spawn_params = &explosion_spawn_params[current_explosion_spawn_params_offset];

        particle_system_spawn(
            sprite->precision_world_xpos + current_explosion_spawn_params->precision_world_xoffset,
            sprite->precision_world_ypos + current_explosion_spawn_params->precision_world_yoffset,
            current_explosion_spawn_params->precision_world_xadd,
            current_explosion_spawn_params->precision_world_yadd,
            PARTICLE_TYPE_GENERIC
        );

        current_explosion_spawn_params_offset++;
        current_explosion_spawn_params_offset &= (EXPLOSION_SPAWN_PARAMS_COUNT - 1);
    }

    sprite->type = SPRITE_TYPE_EXPLOSION;
    explosion_sprite_init_attributes(sprite);
}