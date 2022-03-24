#include <inttypes.h>
#include "game.h"
#include "vbl_handler.h"

void vbl_handler()
{
    // update positions of all particles
    // remove all expired/offscreen particles
    // update player craft in accordance with input
    // update enemy craft in accordance with input
    // check for collisions
    //     destroy 

    game_execute_frame();
}

#endif

