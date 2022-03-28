#ifndef __PLAYER_BEHAVIOUR_H
#define __PLAYER_BEHAVIOUR_H

extern uint32_t player_logical_xpos;
extern uint32_t player_logical_ypos;

void player_behaviour_init_position();
void player_behaviour_update_position();
void player_behaviour_spawn_exhaust_particles();

#endif


