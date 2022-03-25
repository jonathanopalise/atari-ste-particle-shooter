#include "hardware.h"
#include "hardware_playfield.h"
#include "hardware_viewport.h"
#include "logical_viewport.h"
#include "movep.h"
#include "or_table.h"
#include "particle_common.h"
#include "particle_system.h"
#include "viewport.h"

#define VIEWPORT_BYTES_PER_LINE (PIXELS_TO_BYTES(VIEWPORT_WIDTH))
#define PARTICLE_COLOUR 15

/*
 * 1) erase particles from buffer 1 using buffer 1 playfield_draw_offsets
 *    (erase sprites from buffer 1)
 * 2) render particles to buffer 1 and store destination addresses in buffer 1 playfield_draw_offsets
 *    (render sprites to buffer 1)
 * 2) move buffer 1 to physical and buffer 2 to logical
 * 3) buffer 1 is now visible, buffer 2 is now hidden
 * 4) erase particles from buffer 2 using buffer 2 playfield_draw_offsets
 *    (erase sprites from buffer 2)
 * 5) render particles to buffer 2 and store destination addresses in buffer 2 playfield_draw_offsets
 *    (render sprites to buffer 2)
 * 6) move buffer 2 to physical and buffer 1 to logical
 */

void particle_render_draw_particles()
{
    struct Particle *current_particle;
    int16_t logical_viewport_right_xpos;
    int16_t logical_viewport_particle_xpos;
    int16_t logical_viewport_particle_ypos;
    int16_t hardware_viewport_particle_xpos;
    uint32_t hardware_playfield_particle_offset;
    uint16_t or_table_mask_offset;
    uint16_t or_table_colour_offset;
    uint8_t *hardware_playfield_buffer = hidden_hardware_playfield->buffer;

    uint16_t particles_drawn = 0;
    uint32_t *current_particle_draw_offset = hidden_hardware_playfield->particle_draw_offsets;

    // TODO: this needs to go in a common location
    logical_viewport_right_xpos = logical_viewport_left_xpos + (VIEWPORT_WIDTH - 1);

    uint16_t hardware_viewport_left_xpos = hardware_viewport_get_left_xpos();

    current_particle = first_active_particle;
    while (current_particle) {
        logical_viewport_particle_xpos = current_particle->precision_world_xpos >> 16;
        logical_viewport_particle_ypos = current_particle->precision_world_ypos >> 16;

        if (logical_viewport_particle_ypos >= 0 &&
            logical_viewport_particle_ypos < VIEWPORT_HEIGHT &&
            logical_viewport_particle_xpos >= logical_viewport_left_xpos &&
            logical_viewport_particle_ypos <= logical_viewport_right_xpos
        ) {
            // calculate the pixel xpos within the hardware viewport for this particle
            hardware_viewport_particle_xpos = hardware_viewport_left_xpos + 
                (logical_viewport_particle_xpos - logical_viewport_left_xpos);

            // calculate the address of the 16 pixel block within the hardware
            // playfield for this particle
            hardware_playfield_particle_offset =
                (VIEWPORT_BYTES_PER_LINE * logical_viewport_particle_ypos) +
                ((hardware_viewport_particle_xpos >> 4) << 3);

            // is this one of the first 8 pixels in the 16 pixel block?
            // if not, advance the destination address by one byte
            if (!(hardware_viewport_particle_xpos & 8)) {
                hardware_playfield_particle_offset++;
            }

            or_table_mask_offset = (hardware_playfield_particle_offset & 7) << 4;
            or_table_colour_offset = or_table_mask_offset + PARTICLE_COLOUR;

            // movep from parameter 1 into register
            // AND register with parameter 2
            // OR register with parameter 3
            // movep from register into parameter 1
            movep_plot_pixel(
                &hardware_playfield_buffer[hardware_playfield_particle_offset],
                or_table[or_table_mask_offset],
                or_table[or_table_colour_offset]
            );

            *current_particle_draw_offset = hardware_playfield_particle_offset;
            current_particle_draw_offset++;

            particles_drawn++;
        }

        current_particle = current_particle->next;
    }

    hidden_hardware_playfield->particles_drawn = particles_drawn;
}

void particle_render_erase_particles()
{
    uint8_t *hardware_playfield_buffer = hidden_hardware_playfield->buffer;
    uint32_t *current_particle_draw_offset = hidden_hardware_playfield->particle_draw_offsets;
    uint32_t hardware_playfield_particle_offset;

    for (int index = 0; index < hidden_hardware_playfield->particles_drawn; index++) {
        hardware_playfield_particle_offset = *current_particle_draw_offset;

        // movep from parameter 1 into register
        // movep from register into parameter 2
        movep_restore_pixel(
            &hardware_playfield_restore_buffer[hardware_playfield_particle_offset],
            &hardware_playfield_buffer[hardware_playfield_particle_offset]
        );

        current_particle_draw_offset++;
    }
}


