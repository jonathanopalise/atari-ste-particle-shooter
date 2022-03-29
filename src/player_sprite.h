#ifndef __PLAYER_SPRITE_H
#define __PLAYER_SPRITE_H

#include "sprite_system.h"

struct PlayerSpriteAttributes {
    uint16_t joy_fire;
    uint16_t last_joy_fire;
    uint16_t frames_until_next_bullet;
};

void player_sprite_init_attributes(struct Sprite *sprite);

void player_sprite_update_attributes(struct Sprite *sprite);

void player_sprite_handle_enemy_bullet(struct Sprite *sprite);

#endif
