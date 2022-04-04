#ifndef __WAVE_EVENT_H
#define __WAVE_EVENT_H

#include <inttypes.h>

struct WaveEvent {
    int16_t relative_world_xpos;
    int16_t world_ypos;
    uint16_t behaviour_index;
    uint16_t path_index;
    int16_t frames_until_next_event;
};

extern struct WaveEvent wave_events[];

#endif
