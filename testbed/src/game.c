#include "game.h"
#include <core/logger.h>

b8 game_initialize(game* game_inst) {
    ZDEBUG("game_init() called!");
    return TRUE;
}

b8 game_update(game* game_inst, f32 delta_time) {
    // ZDEBUG("game update() called!");
    return TRUE;
}

b8 game_render(game* game_inst, f32 delta_time) {
    // ZDEBUG("game render() called!");
    return TRUE;
}

void game_on_resize(game* game_inst, u32 width, u32 height) {
    ZDEBUG("game on resize() called");
}