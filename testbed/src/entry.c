#include "game.h"
#include <entry.h>

#include <platform/platform.h>

b8 create_game(game* out_game) {
    // app config
    out_game->config.start_pos_x = 100;
    out_game->config.start_pos_y = 100;
    out_game->config.start_width = 1200;
    out_game->config.start_height = 720;
    out_game->config.name = "Zohi Testbed";

    out_game->initialize = game_initialize;
    out_game->update = game_update;
    out_game->render = game_render;
    out_game->on_resize = game_on_resize;

    out_game->state = platform_allocate(sizeof(game_state), FALSE);

    return TRUE;
}