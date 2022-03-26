#include "hardware_playfield.h"
#include "hardware_viewport.h"
#include "logical_viewport.h"
#include "viewport.h"

#define ADDITIONAL_WORDS_PER_LINE (PIXELS_TO_WORDS(HARDWARE_PLAYFIELD_WIDTH) - PIXELS_TO_WORDS(VIEWPORT_WIDTH))

uint16_t hardware_viewport_get_left_xpos()
{
    // TODO: change 640 to something that calculates from constants
    return logical_viewport_left_xpos % 640;
}

void hardware_viewport_handle_vbl()
{
    uint16_t horizontal_offset = hardware_viewport_get_left_xpos();

    uint32_t visible_buffer_address = (uint32_t)visible_hardware_playfield->buffer;
    visible_buffer_address += (horizontal_offset & 0xfffffff0) >> 1; // advance 8 bytes for each 16 pixels

    uint8_t address_high_byte = (uint8_t)((visible_buffer_address >> 16) & 0xff);
    uint8_t address_mid_byte = (uint8_t)((visible_buffer_address >> 8) & 0xff);
    uint8_t address_low_byte = (uint8_t)(visible_buffer_address & 0xff);
    uint8_t additional_words_per_line = ADDITIONAL_WORDS_PER_LINE;
    
    if ((horizontal_offset & 15) != 0) {
        additional_words_per_line -= 4;
    }

    *((volatile uint8_t *)0xffff8201) = address_high_byte;
    *((volatile uint8_t *)0xffff8203) = address_mid_byte;
    *((volatile uint8_t *)0xffff820d) = address_low_byte;

    *((volatile uint8_t *)0xffff820f) = additional_words_per_line;
    *((volatile uint8_t *)0xffff8265) = (uint8_t)(horizontal_offset & 15); // fine pixel scroll

    *((volatile uint8_t *)0xffff8205) = address_high_byte;
    *((volatile uint8_t *)0xffff8207) = address_mid_byte;
    *((volatile uint8_t *)0xffff8209) = address_low_byte;
}
