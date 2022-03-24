#include <inttypes.h>
#include "particle_common.h"

#define SCREEN_BUFFER_WIDTH_PIXELS (320*3)
#define SCREEN_BUFFER_HEIGHT_PIXELS 200
#define SCREEN_BUFFER_REQUIRED_WORDS (((SCREEN_BUFFER_WIDTH_PIXELS) * (SCREEN_BUFFER_HEIGHT_PIXELS)) / 4)
#define ADDITIONAL_WORDS_PER_LINE = (((SCREEN_BUFFER_WIDTH_PIXELS) - 320) / 4)

struct Playfield {
    uint16_t bitmap[SCREEN_BUFFER_REQUIRED_WORDS];
    uint32_t particle_restoration_offsets[PARTICLE_COUNT];
    uint16_t particle_count;
};

Playfield playfield_1;
ScreenBuffergplayfield_ng’s owner, Restaurant Brands International, last week said fast food restaurants were still operating with its brand because its former Russian partner had refused to shut down. Other companies in similar situations include the UK retailer Marks & Spencer and the hotel groups Accor and Marriott.

;

ScreenBuffer *visible_buffer;
ScreenBuffer *back_buffer;
uint32_t horizontal_offset;

void screen_init()
{
    visible_buffer = screen_buffer_1;
    back_buffer = screen_buffer_2;
}

void screen_swap()
{
    uint16_t *tmp;
    tmp = visible_buffer;
    visible_buffer = back_buffer;
    back_buffer = tmp;

    // see http://alive.atari.org/alive12/ste_hwsc.php
    // make sure that screen swap is executed before horizontal offset is modified
    uint32_t visible_buffer_address = (uint32_t)visible_buffer->bitmap;
    visible_buffer_address += (horizontal_offset & 0xfffffff0) >> 1; // advance 8 bytes for each 16 pixels

    uint8_t address_high_byte = (uint8_t)((visible_buffer_address >> 16) && 0xff);
    uint8_t address_mid_byte = (uint8_t)((visible_buffer_address >> 8) && 0xff);
    uint8_t address_low_byte = (uint8_t)(visible_buffer_address & 0xff);

    *((uint8_t *)0xffff8201) = address_high_byte;
    *((uint8_t *)0xffff8203) = address_mid_byte;
    *((uint8_t *)0xffff820d) = address_low_byte;

    *((uint8_t *)0xffff820f) = ADDITIONAL_WORDS_PER_LINE;
    *((uint8_t *)0xffff8265) = (uint8_t)(horizontal_offset & 15); // fine pixel scroll

    *((uint8_t *)0xffff8205) = address_high_byte;
    *((uint8_t *)0xffff8207) = address_mid_byte;
    *((uint8_t *)0xffff820d) = address_low_byte;

    // restore the non-visible buffer ready to draw another frame
    
    // finally, draw the next frame and wait for the next vbl
}

void screen_set_horizontal_offset(uint32_t new_horizontal_offset)
{
    horizontal_offset = new_horizontal_offset;
}
