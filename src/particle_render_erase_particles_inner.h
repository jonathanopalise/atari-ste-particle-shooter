#ifndef __PARTICLE_RENDER_ERASE_PARTICLES_INNER_H
#define __PARTICLE_RENDER_ERASE_PARTICLES_INNER_H

#include <inttypes.h>

void particle_render_erase_particles_inner(
    uint16_t particles_drawn,
    uint32_t *particle_draw_offsets,
    uint8_t *hardware_playfield_restore_buffer,
    uint8_t *hardware_playfield_buffer
);

#endif
