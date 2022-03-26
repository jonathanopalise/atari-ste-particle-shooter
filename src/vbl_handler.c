#include "vbl_handler.h"
#include "hardware_playfield.h"
#include "hardware_viewport.h"

volatile uint16_t waiting_for_vbl;

void vbl_handler()
{
    waiting_for_vbl = 0;

    hardware_playfield_handle_vbl();
    hardware_viewport_handle_vbl();
}

