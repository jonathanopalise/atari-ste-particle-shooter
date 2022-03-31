#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "logical_viewport.h"
#include "sprite_behaviour.h"
#include "sprite_system.h"
#include "sprite_common.h"
#include "hardware_playfield.h"
#include "random.h"
#include "viewport.h"

struct Sprite sprites[SPRITE_COUNT];

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
    struct SpriteBehaviour *sprite_behaviour;

    // TODO: deduplicate: these also exist in particle system
    int32_t precision_logical_viewport_left_xpos = logical_viewport_left_xpos << 16;
    int32_t precision_logical_viewport_right_xpos = precision_logical_viewport_left_xpos + (VIEWPORT_WIDTH << 16);

    while (current_sprite) {
        sprite_behaviour = &sprite_behaviours[current_sprite->type];
        sprite_behaviour->update_attributes(current_sprite);

        if (current_sprite->active) {
            if ((current_sprite->precision_world_ypos < (0 - (SPRITE_HEIGHT << 16))) || 
                (current_sprite->precision_world_ypos > ((HARDWARE_PLAYFIELD_HEIGHT << 16) - 1)) ||
                (current_sprite->precision_world_xpos < (precision_logical_viewport_left_xpos - (SPRITE_WIDTH << 16))) ||
                (current_sprite->precision_world_xpos > precision_logical_viewport_right_xpos)) {
                if (current_sprite->has_been_visible_since_spawn) {
                    current_sprite->active = 0;
                }
            } else {
                current_sprite->has_been_visible_since_spawn = 1;
            }
        }

        current_sprite = current_sprite->next;
    }
}

void sprite_system_update_free_list()
{
    struct Sprite *current_sprite = first_active_sprite;
    struct Sprite **last_unkilled_sprite_next_ptr = &first_active_sprite;
    struct Sprite *tmp_sprite;

    while (current_sprite) {
        if (!current_sprite->active) {
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

void sprite_system_manage_waves()
{
    int32_t precision_logical_viewport_left_xpos = logical_viewport_left_xpos << 16;

    if ((logical_viewport_left_xpos & 63) == 63) {
        sprite_system_spawn(
            precision_logical_viewport_left_xpos + (VIEWPORT_WIDTH << 16),
            random() % ((VIEWPORT_HEIGHT - 16) << 16),
            SPRITE_TYPE_MINE
        );
    }
}

void sprite_system_spawn(int32_t precision_world_xpos, int32_t precision_world_ypos, uint16_t type)
{
    struct Sprite *new_sprite;
    struct Sprite *tmp_sprite;

    if (first_free_sprite) {
        // remove new sprite from start of free list
        new_sprite = first_free_sprite;
        first_free_sprite = new_sprite->next;

        // insert new sprite at start of active list
        tmp_sprite = first_active_sprite;
        first_active_sprite = new_sprite;
        new_sprite->next = tmp_sprite;

        new_sprite->precision_world_xpos = precision_world_xpos;
        new_sprite->precision_world_ypos = precision_world_ypos;
        new_sprite->type = type;
        new_sprite->active = 1;
        new_sprite->has_been_visible_since_spawn = 0;

        sprite_behaviours[type].init_attributes(new_sprite);
    }
}

