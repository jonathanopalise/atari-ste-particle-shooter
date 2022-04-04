#ifndef __PARTICLE_RENDER_DRAW_PARTICLES_INNER_H
#define __PARTICLE_RENDER_DRAW_PARTICLES_INNER_H

#include <inttypes.h>
#include "particle_system.h"

uint16_t particle_render_draw_particles_inner(
    struct Particle *current_particle,
    uint16_t hardware_viewport_left_xpos,
    uint8_t **current_particle_draw_pointer,
    uint8_t *hardware_playfield_buffer,
    uint8_t **ypos_pointers
);

#endif
