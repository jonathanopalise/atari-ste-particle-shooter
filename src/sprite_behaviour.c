#include <stddef.h>
#include "player_sprite.h"
#include "sprite_behaviour.h"

struct SpriteBehaviour sprite_behaviours[] = {
    // type 0 - player
    {
        &player_sprite_init_attributes,
        &player_sprite_update_attributes,
        NULL,
        &player_sprite_handle_enemy_bullet
    }
};
