#include <stddef.h>
#include "collision_detection.h"
#include "particle_system.h"
#include "particle_common.h"
#include "sprite_system.h"
#include "sprite_common.h"
#include "sprite_behaviour.h"

struct Particle *collidable_particle_ptrs[PARTICLE_COUNT + 1];
struct Particle **current_collidable_particle_ptr;
struct Particle *current_collidable_particle;

struct Sprite *collidable_sprite_ptrs[SPRITE_COUNT + 1];
struct Sprite **current_collidable_sprite_ptr;
struct Sprite *current_collidable_sprite;

void collision_detection_handle_player_bullet_collisions()
{
    struct SpriteBehaviour *sprite_behaviour;

    current_collidable_sprite_ptr = collidable_sprite_ptrs;
    struct Sprite *current_sprite = first_active_sprite;
    while (current_sprite) {
        if (current_sprite->active) {
            // TODO: this is likely to be really slow - examine further!
            if (sprite_behaviours[current_sprite->behaviour_index].handle_player_bullet_collision) {
                *current_collidable_sprite_ptr = current_sprite;
                current_collidable_sprite_ptr++;
            }
        }
        current_sprite = current_sprite->next;
    }
    *current_collidable_sprite_ptr = NULL;

    current_collidable_sprite_ptr = collidable_sprite_ptrs;
    current_collidable_sprite = *current_collidable_sprite_ptr;
    while (current_collidable_sprite) {
        current_collidable_particle_ptr = collidable_particle_ptrs;
        current_collidable_particle = *current_collidable_particle_ptr;
        while (current_collidable_particle) {
            if (current_collidable_sprite->active && current_collidable_particle->time_to_live) {
                if ((current_collidable_particle->precision_world_xpos > current_collidable_sprite->precision_world_xpos) &&
                    (current_collidable_particle->precision_world_xpos < (current_collidable_sprite->precision_world_xpos + (SPRITE_WIDTH << 16))) &&
                    (current_collidable_particle->precision_world_ypos > current_collidable_sprite->precision_world_ypos) &&
                    (current_collidable_particle->precision_world_ypos < (current_collidable_sprite->precision_world_ypos + (SPRITE_HEIGHT << 16)))
                ) {
                    sprite_behaviour = &sprite_behaviours[current_collidable_sprite->behaviour_index];
                    if (sprite_behaviour->handle_player_bullet_collision) {
                        sprite_behaviour->handle_player_bullet_collision(current_collidable_sprite);
                        current_collidable_particle->time_to_live = 0;
                    }
                }
            }

            current_collidable_particle_ptr++;
            current_collidable_particle = *current_collidable_particle_ptr;
        }
        current_collidable_sprite_ptr++;
        current_collidable_sprite = *current_collidable_sprite_ptr;
    }
}
