#include <stddef.h>
#include "player_sprite.h"
#include "mine_sprite.h"
#include "explosion_sprite.h"
#include "sprite_behaviour.h"

struct SpriteBehaviour sprite_behaviours[] = {
    // type 0 - player
    {
        NULL,
        &player_sprite_init_attributes,
        &player_sprite_update_attributes,
        NULL,
        &player_sprite_handle_enemy_bullet
    },
    // type 1 - mine
    {
        &mine_sprite_init_type,
        &mine_sprite_init_attributes,
        &mine_sprite_update_attributes,
        &mine_sprite_handle_player_bullet,
        NULL
    },
    // type 2 - explosion
    {
        NULL,
        &explosion_sprite_init_attributes,
        &explosion_sprite_update_attributes,
        NULL,
        NULL
    }
};