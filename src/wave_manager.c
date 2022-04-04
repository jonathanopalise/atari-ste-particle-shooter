#include "wave_event.h"
#include "sprite_system.h"
#include "logical_viewport.h"

struct WaveEvent *next_wave_event;
int16_t frames_until_next_event;

#define FRAMES_UNTIL_FIRST_EVENT 20

void wave_manager_init()
{
    frames_until_next_event = FRAMES_UNTIL_FIRST_EVENT;
    next_wave_event = wave_events;
}

void wave_manager_execute()
{
    if (frames_until_next_event == -1) {
        return;
    }

    frames_until_next_event--;
    while (frames_until_next_event == 0) {
        if (next_wave_event->frames_until_next_event == -1) {
            frames_until_next_event = -1;
            return;
        }

        sprite_system_spawn(
            (logical_viewport_left_xpos + next_wave_event->relative_world_xpos) << 16,
            next_wave_event->world_ypos << 16,
            next_wave_event->behaviour_index,
            next_wave_event->path_index
        );

        frames_until_next_event = next_wave_event->frames_until_next_event;
        next_wave_event++;
    }
}

