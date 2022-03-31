#ifndef __SPRITE_SYSTEM_H
#define __SPRITE_SYSTEM_H

#include <inttypes.h>

#define ADDITIONAL_DATA_SIZE_BYTES 128
#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16

#define SPRITE_IMAGE_PLAYER 0
#define SPRITE_IMAGE_MINE 1

#define SPRITE_TYPE_PLAYER 0
#define SPRITE_TYPE_MINE 1
#define SPRITE_TYPE_EXPLOSION 2

struct Sprite {
	int32_t precision_world_xpos;
	int32_t precision_world_ypos;
    uint16_t type;
    uint16_t image_index;
    uint8_t active;
    uint8_t has_been_visible_since_spawn;
    struct Sprite *next;
    uint8_t additional_data[ADDITIONAL_DATA_SIZE_BYTES];
};

extern struct Sprite *first_active_sprite;

void sprite_system_init();
void sprite_system_update_system();
void sprite_system_manage_waves();
void sprite_system_spawn(int32_t precision_world_xpos, int32_t precision_world_ypos, uint16_t type);

#endif
