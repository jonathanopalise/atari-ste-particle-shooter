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

#define VIEWPORT_BYTES_PER_LINE (PIXELS_TO_BYTES(VIEWPORT_WIDTH))

void sprite_render_draw_sprites()
{
    struct Sprite *current_sprite = first_active_sprite;
    uint8_t *hardware_playfield_buffer = hidden_hardware_playfield->buffer;
    uint8_t **current_sprite_draw_pointer = hidden_hardware_playfield->particle_draw_pointers;
    uint16_t hardware_viewport_left_xpos = hardware_viewport_get_left_xpos();

    while (current_sprite) {
        logical_viewport_sprite_xpos = current_sprite->precision_world_xpos >> 16;
        logical_viewport_sprite_ypos = current_sprite->precision_world_ypos >> 16;

        if (logical_viewport_sprite_ypos >= 0 &&
            logical_viewport_sprite_ypos < VIEWPORT_HEIGHT &&
            logical_viewport_sprite_xpos >= logical_viewport_left_xpos &&
            logical_viewport_sprite_ypos <= logical_viewport_right_xpos
        ) {
            // calculate the pixel xpos within the hardware viewport for this sprite
            hardware_viewport_sprite_xpos = hardware_viewport_left_xpos + 
                (logical_viewport_particle_xpos - logical_viewport_left_xpos);

            // calculate the address of the 16 pixel block within the hardware
            // playfield for this sprite
            hardware_playfield_particle_offset =
                (VIEWPORT_BYTES_PER_LINE * logical_viewport_particle_ypos) +
                ((hardware_viewport_particle_xpos >> 4) << 3);

            *current_sprite_draw_offset = hardware_playfield_particle_offset;
            current_sprite_draw_offset++;
            sprites_drawn++;
        }

        current_sprite = current_sprite->next;
    }

    hidden_hardware_playfield->sprites_drawn = sprites_drawn;
}

void sprite_render_erase_sprites()
{
    particle_render_erase_particles_inner(
        hidden_hardware_playfield->particles_drawn,
        hidden_hardware_playfield->particle_draw_pointers,
        hardware_playfield_restore_buffer,
        hidden_hardware_playfield->buffer
    );
}

