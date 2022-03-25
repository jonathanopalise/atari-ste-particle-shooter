#include "game_loop.h"
#include "hardware_playfield.h"
#include "hardware_viewport.h"
#include "initialise.h"
#include "logical_viewport.h"
#include "particle_render.h"
#include "particle_system.h"
#include "vbl_handler.h"

void game_loop()
{
    particle_system_init();
    hardware_playfield_init();
    initialise();
    logical_viewport_left_xpos = 0;

    while (1) {
        particle_system_update_system();
        particle_render_erase_particles();
        particle_render_draw_particles();

        waiting_for_vbl = 1;
        while (waiting_for_vbl) {}

        hardware_playfield_handle_vbl();
        hardware_viewport_handle_vbl();
        logical_viewport_left_xpos++;
    }
}
