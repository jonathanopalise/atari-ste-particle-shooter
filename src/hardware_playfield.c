#include <inttypes.h>
#include <string.h>
#include "hardware_playfield.h"
#include "hardware_playfield_restore_buffer.h"
#include "logical_playfield.h"
#include "palette.h"

/*
 * hardware playfield xpos offset will range from 0 to 639
 * upon reaching 640 we go back to 0
 * so it's effectively 2 screens wide, with the 3rd screen available only to facilitate wrapping around
 *
 * so:
 * - hardware playfield xpos offset = logical playfield xpos offset % (HARDWARE_PLAYFIELD_WIDTH - 1)
 * - we should only draw particles where particle_logical_xpos >= logical_playfield_xpos
 * - if we do decide to draw a particle, hardware playfield xpos offset = logical playfield xpos offset
 */

struct HardwarePlayfield *hidden_hardware_playfield;
struct HardwarePlayfield *visible_hardware_playfield;

struct HardwarePlayfield hardware_playfield_1;
struct HardwarePlayfield hardware_playfield_2;

void hardware_playfield_handle_vbl()
{
    struct HardwarePlayfield *tmp_hardware_playfield;

    tmp_hardware_playfield = visible_hardware_playfield;
    visible_hardware_playfield = hidden_hardware_playfield;
    hidden_hardware_playfield = tmp_hardware_playfield;
}

void hardware_playfield_reset()
{
    // TODO
}

static void hardware_playfield_init_playfield(struct HardwarePlayfield *hardware_playfield)
{
    memcpy(
        hardware_playfield->buffer,
        hardware_playfield_restore_buffer,
        HARDWARE_PLAYFIELD_BUFFER_SIZE_BYTES
    );

    hardware_playfield->particles_drawn = 0;
    hardware_playfield->sprites_drawn = 0;
}

void hardware_playfield_init()
{
    memcpy((void *)0xffff8240, palette, 32);

    visible_hardware_playfield = &hardware_playfield_1;
    hidden_hardware_playfield = &hardware_playfield_2;

    hardware_playfield_init_playfield(visible_hardware_playfield);
    hardware_playfield_init_playfield(hidden_hardware_playfield);
}


