#include "hardware.h"
#include "hardware_playfield.h"
#include "hardware_playfield_restore_buffer.h"
#include "hardware_playfield_ypos_lookup.h"
#include "hardware_viewport_xpos_lookup.h"
#include "or_table_mask_lookup.h"
#include "hardware_viewport.h"
#include "logical_viewport.h"
#include "movep.h"
#include "particle_render_erase_particles_inner.h"
#include "particle_render_draw_particles_inner.h"
#include "or_table.h"
#include "particle_common.h"
#include "particle_system.h"
#include "viewport.h"

#define VIEWPORT_BYTES_PER_LINE (PIXELS_TO_BYTES(VIEWPORT_WIDTH))
#define PARTICLE_COLOUR 15

void particle_render_draw_particles()
{
    struct Particle *current_particle;
    int16_t logical_viewport_particle_xpos;
    int16_t logical_viewport_particle_ypos;
    int16_t hardware_viewport_particle_xpos;
    uint32_t hardware_playfield_particle_offset;
    uint8_t *hardware_playfield_particle_ptr;
    uint16_t or_table_mask_offset;
    uint16_t or_table_colour_offset;
    uint8_t *hardware_playfield_buffer = hidden_hardware_playfield->buffer;

    uint16_t particles_drawn = 0;
    uint8_t **current_particle_draw_pointer = hidden_hardware_playfield->particle_draw_pointers;

    uint16_t hardware_viewport_left_xpos = hardware_viewport_get_left_xpos();

    current_particle = first_active_particle;


#ifdef FOO
    while (current_particle) {
        logical_viewport_particle_ypos = current_particle->precision_world_ypos >> 16;

        // we're only checking the particle against the top of the screen
        // so make sure that particle system is updated immediately after spawning new particles
        if (logical_viewport_particle_ypos >= 0) {
            logical_viewport_particle_xpos = current_particle->precision_world_xpos >> 16;
            // calculate the pixel xpos within the hardware viewport for this particle
            hardware_viewport_particle_xpos = hardware_viewport_left_xpos + 
                (logical_viewport_particle_xpos - logical_viewport_left_xpos);

            // calculate the address of the 16 pixel block within the hardware
            // playfield for this particle
            // 480 is hardware playfield bytes per line - need constant
            hardware_playfield_particle_offset =
                hardware_playfield_ypos_lookup[logical_viewport_particle_ypos] +
                hardware_viewport_xpos_lookup[hardware_viewport_particle_xpos];

            hardware_playfield_particle_ptr = &hardware_playfield_buffer[hardware_playfield_particle_offset],
            or_table_mask_offset = or_table_mask_lookup[hardware_viewport_particle_xpos];
            or_table_colour_offset = or_table_mask_offset + PARTICLE_COLOUR;

            movep_plot_pixel(
                hardware_playfield_particle_ptr,
                or_table[or_table_mask_offset],
                or_table[or_table_colour_offset]
            );

            *current_particle_draw_pointer++ = hardware_playfield_particle_ptr;
            particles_drawn++;
        }

        current_particle = current_particle->next;
    }
    hidden_hardware_playfield->particles_drawn = particles_drawn;
#endif

    /*hidden_hardware_playfield->particles_drawn = particle_render_draw_particles_inner(
        current_particle,
        hardware_viewport_left_xpos,
        current_particle_draw_pointer,
        hardware_playfield_buffer
    );*/

    hidden_hardware_playfield->particles_drawn = particle_render_draw_particles_inner(
        current_particle,
        hardware_viewport_left_xpos,
        current_particle_draw_pointer,
        hardware_playfield_buffer
    );

    //hidden_hardware_playfield->particles_drawn = 0;
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

