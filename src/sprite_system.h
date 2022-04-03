#ifndef __SPRITE_SYSTEM_H
#define __SPRITE_SYSTEM_H

#include <inttypes.h>

#define ADDITIONAL_DATA_SIZE_BYTES 128
#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16

#define SPRITE_IMAGE_PLAYER 0
#define SPRITE_IMAGE_MINE 1
#define SPRITE_IMAGE_EXPLOSION_1 2
#define SPRITE_IMAGE_EXPLOSION_2 3
#define SPRITE_IMAGE_EXPLOSION_3 4
#define SPRITE_IMAGE_EXPLOSION_4 5
#define SPRITE_IMAGE_EXPLOSION_5 6
#define SPRITE_IMAGE_EXPLOSION_6 7
#define SPRITE_IMAGE_EXPLOSION_7 8
#define SPRITE_IMAGE_EXPLOSION_8 9
#define SPRITE_IMAGE_EXPLOSION_9 10
#define SPRITE_IMAGE_EXPLOSION_10 11
#define SPRITE_IMAGE_EXPLOSION_11 12
#define SPRITE_IMAGE_EXPLOSION_12 13
#define SPRITE_IMAGE_EXPLOSION_13 13

#define SPRITE_BEHAVIOUR_PLAYER 0
#define SPRITE_BEHAVIOUR_MINE 1
#define SPRITE_BEHAVIOUR_EXPLOSION 2

#define SPRITE_PATH_NONE 0
#define SPRITE_PATH_TYPE_1 1
#define SPRITE_PATH_TYPE_2 2

struct Sprite {
	int32_t precision_world_xpos;
	int32_t precision_world_ypos;
    uint16_t image_index;
    uint16_t behaviour_index;
    uint16_t path_index;
    uint16_t frames_alive;
    uint8_t active;
    uint8_t has_been_visible_since_spawn;
    struct Sprite *next;
    uint8_t additional_data[ADDITIONAL_DATA_SIZE_BYTES];
};

extern struct Sprite *first_active_sprite;

void sprite_system_init();
void sprite_system_update_system();
void sprite_system_update_free_list();
void sprite_system_manage_waves();
void sprite_system_spawn(
    int32_t precision_world_xpos,
    int32_t precision_world_ypos,
    uint16_t behaviour_index,
    uint16_t path_index
);

#endif
