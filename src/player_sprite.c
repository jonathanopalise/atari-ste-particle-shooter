#include "initialise.h"
#include "logical_viewport.h"
#include "particle_system.h"
#include "player_sprite.h"
#include "random.h"
#include "sprite_system.h"
#include "viewport.h"

#define BORDER_SIZE 8

static void player_sprite_fire_bullet(struct Sprite *sprite);
static void player_sprite_spawn_exhaust_particles(struct Sprite *sprite);

void player_sprite_init_attributes(struct Sprite *sprite)
{
    struct PlayerSpriteAttributes *player_sprite_attributes;
   
    player_sprite_attributes = (struct PlayerSpriteAttributes *)sprite->additional_data;
    player_sprite_attributes->last_joy_fire = 0;
    player_sprite_attributes->frames_until_next_bullet = 10;

    sprite->image_index = SPRITE_IMAGE_PLAYER;
}

void player_sprite_update_attributes(struct Sprite *sprite)
{
    struct PlayerSpriteAttributes *sprite_attributes = (struct PlayerSpriteAttributes *)sprite->additional_data;

    uint16_t joy_up = joy_data & 1;
    uint16_t joy_down = joy_data & 2;
    uint16_t joy_left = joy_data & 4;
    uint16_t joy_right = joy_data & 8;

    sprite->precision_world_xpos += 1<<16;
    if (sprite_attributes->frames_until_next_bullet > 0) {
        sprite_attributes->frames_until_next_bullet--;
    }

    if (joy_up) {
        if (sprite->precision_world_ypos > (BORDER_SIZE<<16)) {
            sprite->precision_world_ypos -= 1<<16;
        }
    } else if (joy_down) {
        if (sprite->precision_world_ypos < ((VIEWPORT_HEIGHT-(BORDER_SIZE+SPRITE_HEIGHT))<<16)) {
            sprite->precision_world_ypos += 1<<16;
        }
    }

    if (joy_left) {
        if (sprite->precision_world_xpos > ((logical_viewport_left_xpos+BORDER_SIZE)<<16)) {
            sprite->precision_world_xpos -= 2<<16;
        }
    } else if (joy_right) {
        if (sprite->precision_world_xpos < ((logical_viewport_left_xpos+(VIEWPORT_WIDTH-(BORDER_SIZE+SPRITE_WIDTH)))<<16)) {
            sprite->precision_world_xpos += 1<<16;
        }
    }

    sprite_attributes->last_joy_fire = sprite_attributes->joy_fire;
    sprite_attributes->joy_fire = (joy_data >> 7) & 1;

    if ((sprite_attributes->joy_fire && !(sprite_attributes->last_joy_fire)) && !(sprite_attributes->frames_until_next_bullet)) {
        player_sprite_fire_bullet(sprite);
        sprite_attributes->frames_until_next_bullet = 10;
    }

    player_sprite_spawn_exhaust_particles(sprite);
}

void player_sprite_handle_enemy_bullet(struct Sprite *sprite)
{
}

static void player_sprite_fire_bullet(struct Sprite *sprite)
{
    particle_system_spawn(
        sprite->precision_world_xpos + (16 << 16),
        sprite->precision_world_ypos + (8 << 16),
        (1 << 16) *4,
        0
    );
}

static void player_sprite_spawn_exhaust_particles(struct Sprite *sprite)
{
    particle_system_spawn(
        sprite->precision_world_xpos,
        sprite->precision_world_ypos + (6 << 16) + random() % (4 << 16),
        -(48000 + random() % 16000),
        (random() % 24000) - 12000
    );
}

