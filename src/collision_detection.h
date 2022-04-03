#ifndef __COLLISION_DETECTION_H
#define __COLLISION_DETECTION_H

#include "particle_common.h"

extern struct Particle *collidable_particle_ptrs[PARTICLE_COUNT + 1];
extern struct Particle **current_collidable_particle_ptr;
extern struct Particle *current_collidable_particle;

void collision_detection_handle_player_bullet_collisions();

#endif
