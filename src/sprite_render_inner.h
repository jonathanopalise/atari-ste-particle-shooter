#ifndef __SPRITE_RENDER_INNER_H
#define __SPRITE_RENDER_INNER_H

#include <inttypes.h>
#include "hardware_playfield.h"

void sprite_render_inner_draw(
    uint8_t *source,
    uint8_t *destination,
    uint16_t skew,
    uint16_t width
);

void sprite_render_inner_erase(
    uint16_t particles_drawn,
    struct SpriteDrawRecord *sprite_draw_records,
    uint8_t *hardware_playfield_restore_buffer,
    uint8_t *hardware_playfield_buffer
);

#endif
