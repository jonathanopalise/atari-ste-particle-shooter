#include "sprite_system.h"
#include "particle_system.h"

#define EXPLOSION_FRAME_INTERVAL 4

struct ExplosionSpriteAttributes {
    uint16_t current_animation_frame;
    uint16_t frames_until_next_animation_frame;
};

void explosion_sprite_init_attributes(struct Sprite *sprite)
{
    struct ExplosionSpriteAttributes *explosion_sprite_attributes;

    explosion_sprite_attributes = (struct ExplosionSpriteAttributes *)sprite->additional_data;
    explosion_sprite_attributes->current_animation_frame = 0;
    explosion_sprite_attributes->frames_until_next_animation_frame = EXPLOSION_FRAME_INTERVAL;

    sprite->image_index = SPRITE_IMAGE_EXPLOSION_1;
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

