#include "hardware.h"
#include "hardware_playfield.h"
#include "hardware_playfield_restore_buffer.h"
#include "hardware_viewport.h"
#include "logical_viewport.h"
#include "sprite_common.h"
#include "sprite_render_inner.h"
#include "sprite_system.h"
#include "sprite_data.h"
#include "viewport.h"

#define SPRITE_WIDTH_16_PIXELS 0
#define SPRITE_WIDTH_32_PIXELS 1

void sprite_render_draw_sprites()
{
    struct Sprite *current_sprite = first_active_sprite;
    uint8_t *hardware_playfield_buffer = hidden_hardware_playfield->buffer;
    struct SpriteDrawRecord *current_sprite_draw_record = hidden_hardware_playfield->sprite_draw_records;
    uint16_t hardware_viewport_left_xpos = hardware_viewport_get_left_xpos();

    int16_t logical_viewport_sprite_xpos;
    int16_t logical_viewport_sprite_ypos;
    int16_t hardware_viewport_sprite_xpos;
    int16_t logical_viewport_right_xpos = logical_viewport_left_xpos + VIEWPORT_WIDTH;
    uint32_t hardware_playfield_sprite_offset;
    uint8_t *hardware_playfield_sprite_ptr;
    uint16_t sprites_drawn = 0;
    uint16_t sprite_render_skew;

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
                (logical_viewport_sprite_xpos - logical_viewport_left_xpos);

            sprite_render_skew = hardware_viewport_sprite_xpos & 15;

            // calculate the address of the 16 pixel block within the hardware
            // playfield for this sprite
            hardware_playfield_sprite_offset =
                (480 * logical_viewport_sprite_ypos) +
                ((hardware_viewport_sprite_xpos >> 4) << 3);

            hardware_playfield_sprite_ptr = &hardware_playfield_buffer[hardware_playfield_sprite_offset];

            sprite_render_inner_draw(
                sprite_data[current_sprite->image_index], // source
                hardware_playfield_sprite_ptr, // destination
                sprite_render_skew
            );

            current_sprite_draw_record->draw_pointer = hardware_playfield_sprite_ptr;
            current_sprite_draw_record->draw_width = (sprite_render_skew != 0);
            current_sprite_draw_record++;
            sprites_drawn++;
        }

        current_sprite = current_sprite->next;
    }

    hidden_hardware_playfield->sprites_drawn = sprites_drawn;
}

void sprite_render_erase_sprites()
{
    sprite_render_inner_erase(
        hidden_hardware_playfield->sprites_drawn,
        hidden_hardware_playfield->sprite_draw_records,
        hardware_playfield_restore_buffer,
        hidden_hardware_playfield->buffer
    );

    /*uint32_t buffer_difference = hidden_hardware_playfield->buffer - hardware_playfield_restore_buffer;
    struct SpriteDrawRecord *current_sprite_draw_record = hidden_hardware_playfield->sprite_draw_records;
    uint16_t x;
    uint16_t y;

    for (int index = hidden_hardware_playfield->sprites_drawn; index > 0; index--) {
        uint8_t dest_pointer = *current_sprite_draw_record->draw_pointer;
        uint8_t source_pointer = dest_pointer + buffer_difference;

        for (y = 0; y < 15; y++) {
            for (x = 0; x < 2; x++) {
                *((uint32_t *)dest_pointer) = *((uint32_t *)source_pointer);
                source_pointer += 4;
                dest_pointer += 4;
                *((uint32_t *)dest_pointer) = *((uint32_t *)source_pointer);
            }
            source_pointer += (480-8);
            dest_pointer += (480-8);
        }

        current_sprite_draw_record++;
    }*/
}

