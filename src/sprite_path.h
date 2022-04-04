#ifndef __SPRITE_PATH_H
#define __SPRITE_PATH_H

#include "sprite_system.h"

struct SpritePath {
    void (*update_position)(struct Sprite *sprite);
};

extern struct SpritePath sprite_paths[];

void sprite_path_none_update_position(struct Sprite *sprite);
void sprite_path_1_update_position(struct Sprite *sprite);
void sprite_path_2_update_position(struct Sprite *sprite);
void sprite_path_3_update_position(struct Sprite *sprite);
void sprite_path_4_update_position(struct Sprite *sprite);

#endif
