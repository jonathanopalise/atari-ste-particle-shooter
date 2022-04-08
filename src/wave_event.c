#include "sprite_system.h"
#include "viewport.h"
#include "wave_event.h"

#define WAVE_END -1

struct WaveEvent wave_events[] = {
    {
        VIEWPORT_WIDTH + 16,
        54,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_1,
        0
    },
    {
        VIEWPORT_WIDTH + 16,
        132,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_1,
        5
    },
    {
        VIEWPORT_WIDTH + 16,
        72,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_1,
        0
    },
    {
        VIEWPORT_WIDTH + 16,
        112,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_1,
        5
    },
    {
        VIEWPORT_WIDTH + 16,
        92,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_1,
        30
    },
    // next
    {
        VIEWPORT_WIDTH + 16,
        32,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        32,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        32,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        32,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        32,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        32,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        24
    },
    // next
    {
        VIEWPORT_WIDTH + 16,
        148,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        148,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        148,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        148,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        148,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        8
    },
    {
        VIEWPORT_WIDTH + 16,
        148,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        24
    },

    // next
    {
        VIEWPORT_WIDTH + 16,
        92,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        0
    },
    {
        VIEWPORT_WIDTH + 16,
        52,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        0
    },
    {
        VIEWPORT_WIDTH + 16,
        12,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        0
    },
    {
        VIEWPORT_WIDTH + 16,
        132,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        0
    },
    {
        VIEWPORT_WIDTH + 16,
        172,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        24
    },

    // next
    {
        VIEWPORT_WIDTH + 16,
        42,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        10
    },
    {
        VIEWPORT_WIDTH + 16,
        142,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        10
    },
    {
        VIEWPORT_WIDTH + 16,
        42,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        10
    },
    {
        VIEWPORT_WIDTH + 16,
        142,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        10
    },
    {
        VIEWPORT_WIDTH + 16,
        42,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        10
    },
    {
        VIEWPORT_WIDTH + 16,
        142,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        10
    },
    {
        VIEWPORT_WIDTH + 16,
        42,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        20
    },
    {
        VIEWPORT_WIDTH + 16,
        142,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        20
    },
    {
        VIEWPORT_WIDTH + 16,
        42,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        20
    },
    {
        VIEWPORT_WIDTH + 16,
        142,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_3,
        20
    },


    /*{
        VIEWPORT_WIDTH + 16,
        60,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_1,
        50
    },
    {
        VIEWPORT_WIDTH + 16,
        80,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_1,
        50
    },
    {
        VIEWPORT_WIDTH + 16,
        100,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_2,
        50
    },
    {
        VIEWPORT_WIDTH + 16,
        120,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_1,
        50
    },
    {
        VIEWPORT_WIDTH + 16,
        140,
        SPRITE_BEHAVIOUR_MINE,
        SPRITE_PATH_TYPE_1,
        50
    },*/
    {
        0,
        0,
        0,
        0,
        WAVE_END
    }
};
