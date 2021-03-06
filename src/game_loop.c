#include "game_loop.h"
#include "hardware_playfield.h"
#include "hardware_viewport.h"
#include "initialise.h"
#include "logical_viewport.h"
#include "particle_render.h"
#include "particle_system.h"
#include "sprite_render.h"
#include "sprite_system.h"
#include "wave_manager.h"
#include "collision_detection.h"
#include "vbl_handler.h"

void game_loop()
{
    particle_system_init();
    sprite_system_init();
    wave_manager_init();
    hardware_playfield_init();
    initialise();
    logical_viewport_left_xpos = 0;

    while (1) {
        // this shouldn't be here but it doesn't like being above
        if (logical_viewport_left_xpos == 1) {
            sprite_system_spawn(160 << 16, 100 << 16, SPRITE_BEHAVIOUR_PLAYER, SPRITE_PATH_NONE);
        }

        //sprite_system_manage_waves();
        wave_manager_execute();
        // is it possible that collision detection can spawn particles out of bounds?
        collision_detection_handle_player_bullet_collisions();
        sprite_system_update_system();
        particle_system_update_system();
        *((volatile uint16_t *)0xffff8240) = 0x666; // grey
        sprite_system_update_free_list();
        particle_system_update_free_list();
        *((volatile uint16_t *)0xffff8240) = 0x040; // green
        particle_render_erase_particles();
        sprite_render_erase_sprites();
        *((volatile uint16_t *)0xffff8240) = 0x004; // blue
        particle_render_draw_particles();
        sprite_render_draw_sprites();

        *((volatile uint16_t *)0xffff8240) = 0x000;

        waiting_for_vbl = 1;
        while (waiting_for_vbl) {}
        hardware_playfield_handle_vbl();
        hardware_viewport_handle_vbl();

        *((volatile uint16_t *)0xffff8240) = 0x400; // red

        logical_viewport_left_xpos++;
    }
}
