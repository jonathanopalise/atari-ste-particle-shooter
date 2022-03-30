#include "game_loop.h"
#include "hardware_playfield.h"
#include "initialise.h"
#include "logical_viewport.h"
#include "particle_render.h"
#include "particle_system.h"
#include "sprite_render.h"
#include "sprite_system.h"
#include "vbl_handler.h"

void game_loop()
{
    particle_system_init();
    sprite_system_init();
    sprite_system_spawn(160 << 16, 100 << 16, SPRITE_TYPE_PLAYER);
    hardware_playfield_init();
    initialise();
    logical_viewport_left_xpos = 0;

    while (1) {
        sprite_system_manage_waves();
        sprite_system_update_system();
        particle_system_update_system();
        *((volatile uint16_t *)0xffff8240) = 0x040; // green
        particle_render_erase_particles();
        sprite_render_erase_sprites();
        *((volatile uint16_t *)0xffff8240) = 0x004; // blue
        particle_render_draw_particles();
        sprite_render_draw_sprites();

        *((volatile uint16_t *)0xffff8240) = 0x000;

        waiting_for_vbl = 1;
        while (waiting_for_vbl) {}

        *((volatile uint16_t *)0xffff8240) = 0x400; // red

        logical_viewport_left_xpos++;
    }
}
