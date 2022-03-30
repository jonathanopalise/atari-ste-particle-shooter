#ifndef __HARDWARE_PLAYFIELD_H
#define __HARDWARE_PLAYFIELD_H

#include <inttypes.h>
#include "hardware.h"
#include "particle_common.h"
#include "sprite_common.h"

#define HARDWARE_PLAYFIELD_WIDTH 960
#define HARDWARE_PLAYFIELD_HEIGHT 200
#define HARDWARE_PLAYFIELD_BUFFER_SIZE_BYTES (PIXELS_TO_BYTES(HARDWARE_PLAYFIELD_WIDTH * HARDWARE_PLAYFIELD_HEIGHT))

struct SpriteDrawRecord {
    uint8_t *draw_pointer;
    uint16_t draw_width;
};

struct HardwarePlayfield {
   uint8_t buffer[HARDWARE_PLAYFIELD_BUFFER_SIZE_BYTES];
   uint8_t *particle_draw_pointers[PARTICLE_COUNT];
   uint16_t particles_drawn;
   struct SpriteDrawRecord sprite_draw_records[SPRITE_COUNT];
   uint16_t sprites_drawn;
};

extern struct HardwarePlayfield *hidden_hardware_playfield;
extern struct HardwarePlayfield *visible_hardware_playfield;

void hardware_playfield_handle_vbl();
void hardware_playfield_init();

#endif
