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
    /*current_collidable_particle_ptr = collidable_particle_ptrs;
    struct Particle *current_particle = first_active_particle;
    while (current_particle) {
        if (current_particle->active && current_particle->type == PARTICLE_TYPE_PLAYER_BULLET) {
            *current_collidable_particle_ptr = current_particle;
            current_collidable_particle_ptr++;
        }
        current_particle = current_particle->next;
    }
    *current_collidable_particle_ptr = NULL;*/

    current_collidable_sprite_ptr = collidable_sprite_ptrs;
    struct Sprite *current_sprite = first_active_sprite;
    while (current_sprite) {
        if (current_sprite->active) {
            // TODO: this is likely to be really slow - examine further!
            if (sprite_behaviours[current_sprite->type].handle_player_bullet_collision) {
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
            if (current_collidable_sprite->active && current_collidable_particle->active) {
                if ((current_collidable_particle->precision_world_xpos > current_collidable_sprite->precision_world_xpos) &&
                    (current_collidable_particle->precision_world_xpos < (current_collidable_sprite->precision_world_xpos + (SPRITE_WIDTH << 16))) &&
                    (current_collidable_particle->precision_world_ypos > current_collidable_sprite->precision_world_ypos) &&
                    (current_collidable_particle->precision_world_ypos < (current_collidable_sprite->precision_world_ypos + (SPRITE_HEIGHT << 16)))
                ) {
                    sprite_behaviours[current_collidable_sprite->type].handle_player_bullet_collision(current_collidable_sprite);
                    current_collidable_particle->active = 0;
                }
            }

            current_collidable_particle_ptr++;
            current_collidable_particle = *current_collidable_particle_ptr;
        }
        current_collidable_sprite_ptr++;
        current_collidable_sprite = *current_collidable_sprite_ptr;
    }
}
