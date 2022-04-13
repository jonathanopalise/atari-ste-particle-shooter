#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "collision_detection.h"
#include "logical_viewport.h"
#include "particle_system.h"
#include "hardware_playfield.h"
#include "random.h"
#include "viewport.h"

#define GRAVITY 1200

struct Particle particles[PARTICLE_COUNT];

struct Particle *first_free_particle;

void particle_system_init()
{
    first_free_particle = particles;
    for (int index = 0; index < PARTICLE_COUNT - 1; index++) {
        particles[index].time_to_live = 0;
        particles[index].next = &particles[index + 1];
    }

    particles[PARTICLE_COUNT - 1].next = NULL;
}

void particle_system_update_system()
{
    int32_t precision_logical_viewport_left_xpos = logical_viewport_left_xpos << 16;
    int32_t precision_logical_viewport_right_xpos = precision_logical_viewport_left_xpos + (VIEWPORT_WIDTH << 16);

    current_collidable_particle_ptr = collidable_particle_ptrs;
    struct Particle *current_particle = particles;

    for (uint16_t index = 0; index < PARTICLE_COUNT; index++) {
        if (current_particle->time_to_live > 0) {
            current_particle->time_to_live--;

            current_particle->precision_world_yadd += GRAVITY;
            current_particle->precision_world_ypos += current_particle->precision_world_yadd;
            if ((current_particle->precision_world_ypos > ((HARDWARE_PLAYFIELD_HEIGHT << 16) - 1)) ||
                current_particle->precision_world_ypos < 0
            ) {
                current_particle->time_to_live = 0;
            } else {
                current_particle->precision_world_xpos += current_particle->precision_world_xadd;
                if (current_particle->precision_world_xpos < precision_logical_viewport_left_xpos ||
                    current_particle->precision_world_xpos > precision_logical_viewport_right_xpos
                ) {
                    current_particle->time_to_live = 0;
                }
            }

            if (current_particle->type == PARTICLE_TYPE_PLAYER_BULLET) {
                *current_collidable_particle_ptr = current_particle;
                current_collidable_particle_ptr++;
            }
        }

        current_particle++;
    }

    *current_collidable_particle_ptr = NULL;
}

void particle_system_update_free_list()
{
    struct Particle *current_particle = particles;

    for (uint16_t index = 0; index < PARTICLE_COUNT; index++) {
        if (current_particle->time_to_live == 0) {
            current_particle->next = first_free_particle;
            first_free_particle = current_particle;
        }

        current_particle++;
    }
}

void particle_system_spawn(
    int32_t precision_world_xpos,
    int32_t precision_world_ypos,
    int32_t precision_world_xadd,
    int32_t precision_world_yadd,
    uint16_t time_to_live,
    uint16_t type
) {
    struct Particle *new_particle;

    if (first_free_particle) {
        // remove new particle from start of free list
        new_particle = first_free_particle;
        first_free_particle = new_particle->next;

        new_particle->precision_world_xpos = precision_world_xpos;
        new_particle->precision_world_ypos = precision_world_ypos;
        new_particle->precision_world_xadd = precision_world_xadd;
        new_particle->precision_world_yadd = precision_world_yadd;
        new_particle->time_to_live = time_to_live;
        new_particle->active = 0;
        new_particle->type = type;
    }
}

