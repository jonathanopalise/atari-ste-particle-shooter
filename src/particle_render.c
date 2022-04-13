#include "hardware.h"
#include "hardware_playfield.h"
#include "hardware_playfield_restore_buffer.h"
#include "hardware_viewport.h"
#include "logical_viewport.h"
#include "particle_render_erase_particles_inner.h"
#include "particle_render_draw_particles_inner.h"
#include "particle_common.h"
#include "particle_system.h"
#include "viewport.h"

void particle_render_draw_particles()
{
    struct Particle *current_particle = particles;
    uint8_t *hardware_playfield_buffer = hidden_hardware_playfield->buffer;
    uint8_t **current_particle_draw_pointer = hidden_hardware_playfield->particle_draw_pointers;
    uint16_t hardware_viewport_left_xpos = hardware_viewport_get_left_xpos();

    hidden_hardware_playfield->particles_drawn = particle_render_draw_particles_inner(
        current_particle,
        hardware_viewport_left_xpos,
        current_particle_draw_pointer,
        hardware_playfield_buffer,
        hidden_hardware_playfield->ypos_pointers
    );
}

void particle_render_erase_particles()
{
    particle_render_erase_particles_inner(
        hidden_hardware_playfield->particles_drawn,
        hidden_hardware_playfield->particle_draw_pointers,
        hardware_playfield_restore_buffer,
        hidden_hardware_playfield->buffer
    );
}

