#ifndef __MINE_SPRITE_H
#define __MINE_SPRITE_H

#include "sprite_system.h"

void mine_sprite_init_attributes(struct Sprite *sprite);

void mine_sprite_update_attributes(struct Sprite *sprite);

void mine_sprite_handle_player_bullet(struct Sprite *sprite);

#endif
