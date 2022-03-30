#ifndef __SPRITE_RENDER_INNER_H
#define __SPRITE_RENDER_INNER_H

#include <inttypes.h>

void sprite_render_inner_draw(
    uint8_t *source,
    uint8_t *destination,
    uint16_t skew,
    uint16_t width
);

#endif
