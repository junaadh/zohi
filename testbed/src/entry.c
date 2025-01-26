#include "game.h"
#include <entry.h>

#include <core/zmemory.h>

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

    out_game->state = zallocate(sizeof(game_state), MEMORY_TAG_GAME);

    return TRUE;
}