#include "logical_viewport.h"
#include "particle_system.h"
#include "player_behaviour.h"
#include "initialise.h"
#include "random.h"
#include "viewport.h"

#define PLAYER_SPRITE_WIDTH 16
#define PLAYER_SPRITE_HEIGHT 16
#define BORDER_SIZE 16

uint32_t player_logical_xpos;
uint32_t player_logical_ypos;

void player_behaviour_init_position()
{
    player_logical_xpos = 160 << 16;
    player_logical_ypos = 100 << 16;
}

void player_behaviour_update_position()
{
    uint16_t joy_up = joy_data & 1;
    uint16_t joy_down = joy_data & 2;
    uint16_t joy_left = joy_data & 4;
    uint16_t joy_right = joy_data & 8;
    uint16_t joy_fire = (joy_data >> 7) & 1;

    player_logical_xpos += 1<<16;

    if (joy_up) {
        if (player_logical_ypos > (BORDER_SIZE<<16)) {
            player_logical_ypos -= 1<<16;
        }
    } else if (joy_down) {
        if (player_logical_ypos < ((VIEWPORT_HEIGHT-(BORDER_SIZE+PLAYER_SPRITE_HEIGHT))<<16)) {
            player_logical_ypos += 1<<16;
        }
    }

    if (joy_left) {
        if (player_logical_xpos > ((logical_viewport_left_xpos+BORDER_SIZE)<<16)) {
            player_logical_xpos -= 1<<16;
        }
    } else if (joy_right) {
        if (player_logical_xpos < ((logical_viewport_left_xpos+(VIEWPORT_WIDTH-(BORDER_SIZE+PLAYER_SPRITE_WIDTH)))<<16)) {
            player_logical_xpos += 1<<16;
        }
    }
}

void player_behaviour_spawn_exhaust_particles()
{
    particle_system_spawn(
        player_logical_xpos,
        player_logical_ypos,
        -(48000 + random() % 16000),
        (random() % 12000) - 6000
    );
}
