#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "logical_viewport.h"
#include "particle_system.h"
#include "particle_common.h"
#include "hardware_playfield.h"
#include "random.h"
#include "viewport.h"
#include "tigr.h"

#define GRAVITY 400

struct Particle particles[PARTICLE_COUNT];

struct Particle *first_active_particle;
struct Particle *first_free_particle;

void particle_system_init()
{
    first_active_particle = NULL;

    first_free_particle = particles;
    for (int index = 0; index < PARTICLE_COUNT - 1; index++) {
		particles[index].next = &particles[index + 1];
    }

    particles[PARTICLE_COUNT - 1].next = NULL;
}

void particle_system_update_system()
{
    struct Particle *tmp_particle;
    struct Particle *current_particle = first_active_particle;
    struct Particle **last_unkilled_particle_next_ptr = &first_active_particle;
    uint16_t particle_killed;

    int32_t precision_logical_viewport_left_xpos = logical_viewport_left_xpos << 16;
    int32_t precision_logical_viewport_right_xpos = precision_logical_viewport_left_xpos + (VIEWPORT_WIDTH << 16);

    while (current_particle) {
        particle_killed = 0;

        current_particle->time_to_live--;
        if (current_particle->time_to_live == 0) {
            particle_killed = 1;
        } else {
            // update properties of particle
            current_particle->precision_world_yadd += GRAVITY;
            current_particle->precision_world_ypos += current_particle->precision_world_yadd;
            if (current_particle->precision_world_ypos > ((HARDWARE_PLAYFIELD_HEIGHT << 16) - 1)) {
                particle_killed = 1;
            } else {
                current_particle->precision_world_xpos += current_particle->precision_world_xadd;
                if (current_particle->precision_world_xpos < precision_logical_viewport_left_xpos) {
                    particle_killed = 1;
                } else if (current_particle->precision_world_xpos > precision_logical_viewport_right_xpos) {
                    particle_killed = 1;
                }
            }
        }

        if (particle_killed) {
            // remove dead particle from the active list
            *last_unkilled_particle_next_ptr = current_particle->next;

            // add dead particle to the start of the free list
            tmp_particle = first_free_particle;
            first_free_particle = current_particle;
            current_particle = current_particle->next; 
            first_free_particle->next = tmp_particle;
        } else {
            last_unkilled_particle_next_ptr = &(current_particle->next);
            current_particle = current_particle->next;
        }
    }
}

void particle_system_spawn(
    int32_t precision_world_xpos,
    int32_t precision_world_ypos,
    int32_t precision_world_xadd,
    int32_t precision_world_yadd
) {
    struct Particle *new_particle;
    struct Particle *tmp_particle;

    if (first_free_particle) {
        // remove new particle from start of free list
        new_particle = first_free_particle;
        first_free_particle = new_particle->next;

        // insert new particle at start of active list
        tmp_particle = first_active_particle;
        first_active_particle = new_particle;
        new_particle->next = tmp_particle;

        new_particle->precision_world_xpos = precision_world_xpos;
        new_particle->precision_world_ypos = precision_world_ypos;
        new_particle->precision_world_xadd = precision_world_xadd;
        new_particle->precision_world_yadd = precision_world_yadd;
        new_particle->time_to_live = 63;
    }
}



/*int main(int argc, char *argv[])
{
    int frame_index;

    struct Particle *current_particle;
    Tigr *screen = tigrWindow(320, 200, "Hello", 0);
    particle_system_init();
    while (!tigrClosed(screen))
    {
        particle_system_spawn(160<<16, 100<<16);
        tigrClear(screen, tigrRGB(0x10, 0x10, 0x10));
        current_particle = first_active_particle;

        while (current_particle) {
            tigrPlot(
                screen,
                current_particle->precision_world_xpos >> 16,
                current_particle->precision_world_ypos >> 16,
                tigrRGB(
                    0xff,
                    0xff,
                    0xff
                )
            );

            current_particle = current_particle->next;
        }
        
        particle_system_update_system();
        tigrUpdate(screen);
        frame_index++;
    }
    tigrFree(screen);
}*/
