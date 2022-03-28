#include "game_loop.h"
#include "hardware_playfield.h"
#include "initialise.h"
#include "logical_viewport.h"
#include "particle_render.h"
#include "particle_system.h"
#include "player_behaviour.h"
#include "vbl_handler.h"

void game_loop()
{
    player_behaviour_init_position();
    particle_system_init();
    hardware_playfield_init();
    initialise();
    logical_viewport_left_xpos = 0;

    while (1) {
        player_behaviour_update_position();
        particle_system_update_system();
        //*((volatile uint16_t *)0xffff8240) = 0x040; // green
        particle_render_erase_particles();
        //*((volatile uint16_t *)0xffff8240) = 0x004; // blue
        particle_render_draw_particles();

        //*((volatile uint16_t *)0xffff8240) = 0x000;

        waiting_for_vbl = 1;
        while (waiting_for_vbl) {}

        //*((volatile uint16_t *)0xffff8240) = 0x400; // red

        /*if (logical_viewport_left_xpos & 1) {
            particle_system_spawn(((logical_viewport_left_xpos + 160) << 16), 100 << 16);
        }*/
        player_behaviour_spawn_exhaust_particles();

        logical_viewport_left_xpos++;
    }
}
