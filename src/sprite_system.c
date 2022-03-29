#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "logical_viewport.h"
#include "sprite_behaviour.h"
#include "sprite_system.h"
#include "hardware_playfield.h"
#include "viewport.h"

struct Sprite sprites[PARTICLE_COUNT];

struct Sprite *first_active_sprite;
struct Sprite *first_free_sprite;

void sprite_system_init()
{
    first_active_sprite = NULL;

    first_free_sprite = sprites;
    for (int index = 0; index < SPRITE_COUNT - 1; index++) {
		sprites[index].next = &sprites[index + 1];
    }

    sprites[SPRITE_COUNT - 1].next = NULL;
}

void sprite_system_update_system()
{
    struct Sprite *current_sprite = first_active_sprite;
    struct Sprite **last_unkilled_sprite_next_ptr = &first_active_sprite;
    struct Sprite *tmp_sprite;
    struct SpriteBehaviour *sprite_behaviour;

    while (current_sprite) {
        sprite_behaviour = &sprite_behaviours[current_sprite->type];
        sprite_behaviour->update_attributes(current_sprite);

        if (current_sprite->active) {
            // remove dead sprite from the active list
            *last_unkilled_sprite_next_ptr = current_sprite->next;

            // add dead sprite to the start of the free list
            tmp_sprite = first_free_sprite;
            first_free_sprite = current_sprite;
            current_sprite = current_sprite->next; 
            first_free_sprite->next = tmp_sprite;
        } else {
            last_unkilled_sprite_next_ptr = &(current_sprite->next);
            current_sprite = current_sprite->next;
        }
    }
}

void sprite_system_spawn(int32_t precision_world_xpos, int32_t precision_world_ypos, uint16_t type)
{
    struct Sprite *new_sprite;
    struct Sprite *tmp_sprite;

    if (first_free_sprite) {
        // remove new particle from start of free list
        new_sprite = first_free_sprite;
        first_free_sprite = new_sprite->next;

        // insert new sprite at start of active list
        tmp_sprite = first_active_sprite;
        first_active_sprite = new_sprite;
        new_sprite->next = tmp_sprite;

        new_sprite->precision_world_xpos = precision_world_xpos;
        new_sprite->precision_world_ypos = precision_world_ypos;
        new_sprite->type = type;
    }
}

