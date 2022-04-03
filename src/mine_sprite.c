#include "inttypes.h"
#include "sprite_system.h"
#include "mine_sprite.h"

void mine_sprite_init_attributes(struct Sprite *sprite)
{
    sprite->image_index = SPRITE_IMAGE_MINE;
}

void mine_sprite_update_attributes(struct Sprite *sprite)
{
}

void mine_sprite_handle_player_bullet(struct Sprite *sprite)
{
    explosion_sprite_init_attributes(sprite);
}
