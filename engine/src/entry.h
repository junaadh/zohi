#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "core/zmemory.h"
#include "game_types.h"

// externally defined function to create a game
extern b8 create_game(game *game);

/**
 * entry point into the game
 */
int main(void) {
    initialize_memory();

    // rqst the game instance from the application
    game game_inst;
    if (!create_game(&game_inst)) {
        ZFATAL("Could'nt create game");
        return -1;
    }

    // ensure function pointers exist
    if (!game_inst.render || !game_inst.on_resize || !game_inst.update || !game_inst.initialize) {
        ZFATAL("The game's function pointers must be assigned");
        return -2;
    }

    // initialization
    if (!application_create(&game_inst)) {
        ZINFO("Application failed to create!\n");
        return 1;
    }

    // gameloop
    if (!application_run()) {
        ZINFO("Application did not shutdown gracefully!\n");
        return 2;
    }

    shutdown_memory();
    return 0;
}
