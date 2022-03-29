#ifndef __SPRITE_BEHAVIOUR_H
#define __SPRITE_BEHAVIOUR_H

#include "sprite_system.h"

struct SpriteBehaviour {
    void (*init_attributes)(struct Sprite *sprite);
    void (*update_attributes)(struct Sprite *sprite);
    void (*handle_player_bullet_collision)(struct Sprite *sprite);
    void (*handle_enemy_bullet_collision)(struct Sprite *sprite);
};

extern struct SpriteBehaviour sprite_behaviours[];

#endif
