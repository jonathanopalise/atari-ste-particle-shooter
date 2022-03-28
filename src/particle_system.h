#ifndef __PARTICLE_SYSTEM_H
#define __PARTICLE_SYSTEM_H

#include <inttypes.h>

struct Particle {
	int32_t precision_world_xpos; // offset 0
	int32_t precision_world_ypos; // offset 4
	int32_t precision_world_xadd; // offset 8
	int32_t precision_world_yadd; // offset 12
    int16_t time_to_live; // offset 16
    struct Particle *next; // offset 18 
};

extern struct Particle *first_active_particle;

void particle_system_init();
void particle_system_update_system();
void particle_system_spawn(
    int32_t precision_world_xpos,
    int32_t precision_world_ypos,
    int32_t precision_world_xadd,
    int32_t precision_world_yadd
);

#endif
