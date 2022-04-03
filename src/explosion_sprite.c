#include "sprite_system.h"
#include "particle_system.h"
#include "random.h"

#define EXPLOSION_FRAME_INTERVAL 4
#define EXPLOSION_SPAWN_PARAMS_COUNT 128

uint16_t current_explosion_spawn_params_offset;

struct ExplosionSpawnParams {
    int32_t precision_world_xoffset;
    int32_t precision_world_yoffset;
    int32_t precision_world_xadd;
    int32_t precision_world_yadd;
    uint16_t time_to_live;
};

struct ExplosionSpriteAttributes {
    uint16_t current_animation_frame;
    uint16_t frames_until_next_animation_frame;
};

struct ExplosionSpawnParams explosion_spawn_params[EXPLOSION_SPAWN_PARAMS_COUNT];

void explosion_sprite_init_type()
{
    for (uint16_t index = 0; index < EXPLOSION_SPAWN_PARAMS_COUNT; index++) {
        explosion_spawn_params[index].precision_world_xoffset = (6 << 16) + random() % (4 << 16);
        explosion_spawn_params[index].precision_world_yoffset = (6 << 16) + random() % (4 << 16);
        explosion_spawn_params[index].precision_world_xadd = (random() % 80000);
        explosion_spawn_params[index].precision_world_yadd = -(random() % 80000);
        explosion_spawn_params[index].time_to_live = 48 + random() % 16;
    }

    current_explosion_spawn_params_offset = 0;
}

void explosion_sprite_init_attributes(struct Sprite *sprite)
{
    struct ExplosionSpriteAttributes *explosion_sprite_attributes;
    struct ExplosionSpawnParams *current_explosion_spawn_params;

    explosion_sprite_attributes = (struct ExplosionSpriteAttributes *)sprite->additional_data;
    explosion_sprite_attributes->current_animation_frame = 0;
    explosion_sprite_attributes->frames_until_next_animation_frame = EXPLOSION_FRAME_INTERVAL;

    sprite->image_index = SPRITE_IMAGE_EXPLOSION_1;

    for (uint16_t index = 0; index < 25; index++) {
        current_explosion_spawn_params = &explosion_spawn_params[current_explosion_spawn_params_offset];

        particle_system_spawn(
            sprite->precision_world_xpos + current_explosion_spawn_params->precision_world_xoffset,
            sprite->precision_world_ypos + current_explosion_spawn_params->precision_world_yoffset,
            current_explosion_spawn_params->precision_world_xadd,
            current_explosion_spawn_params->precision_world_yadd,
            current_explosion_spawn_params->time_to_live,
            PARTICLE_TYPE_GENERIC
        );

        current_explosion_spawn_params_offset++;
        current_explosion_spawn_params_offset &= (EXPLOSION_SPAWN_PARAMS_COUNT - 1);
    }
}

void explosion_sprite_update_attributes(struct Sprite *sprite)
{
    struct ExplosionSpriteAttributes *explosion_sprite_attributes;

    explosion_sprite_attributes = (struct ExplosionSpriteAttributes *)sprite->additional_data;
    explosion_sprite_attributes->frames_until_next_animation_frame--;

    if (explosion_sprite_attributes->frames_until_next_animation_frame == 0) {
        if (explosion_sprite_attributes->current_animation_frame == SPRITE_IMAGE_EXPLOSION_11) {
            sprite->active = 0;
        } else {
            explosion_sprite_attributes->current_animation_frame++;
            sprite->image_index = SPRITE_IMAGE_EXPLOSION_1 + explosion_sprite_attributes->current_animation_frame;
            explosion_sprite_attributes->frames_until_next_animation_frame = EXPLOSION_FRAME_INTERVAL;
        }
    }
}

