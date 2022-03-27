#ifndef __HARDWARE_PLAYFIELD_H
#define __HARDWARE_PLAYFIELD_H

#include <inttypes.h>
#include "hardware.h"
#include "particle_common.h"

#define HARDWARE_PLAYFIELD_WIDTH 960
#define HARDWARE_PLAYFIELD_HEIGHT 200
#define HARDWARE_PLAYFIELD_BUFFER_SIZE_BYTES (PIXELS_TO_BYTES(HARDWARE_PLAYFIELD_WIDTH * HARDWARE_PLAYFIELD_HEIGHT))

struct HardwarePlayfield {
   uint8_t buffer[HARDWARE_PLAYFIELD_BUFFER_SIZE_BYTES];
   uint8_t *particle_draw_pointers[PARTICLE_COUNT];
   uint16_t particles_drawn;
};

extern struct HardwarePlayfield *hidden_hardware_playfield;
extern struct HardwarePlayfield *visible_hardware_playfield;

//extern uint8_t hardware_playfield_restore_buffer[HARDWARE_PLAYFIELD_BUFFER_SIZE_BYTES];

void hardware_playfield_handle_vbl();
void hardware_playfield_init();

#endif
