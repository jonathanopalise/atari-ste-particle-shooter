#ifndef __PARTICLE_SYSTEM_H
#define __PARTICLE_SYSTEM_H

#include <inttypes.h>

struct Particle {
	int32_t precision_world_xpos;
	int32_t precision_world_ypos;
	int32_t precision_world_xadd;
	int32_t precision_world_yadd;
    int16_t time_to_live;
    struct Particle *next;
};

extern struct Particle *first_active_particle;

void particle_system_init();
void particle_system_update_system();
void particle_system_spawn(int32_t precision_world_xpos, int32_t precision_world_ypos);

#endif
