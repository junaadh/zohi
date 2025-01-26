#pragma once

#include "core/application.h"

/**
 * represents the basic game state in a game
 * called for creation by the application
 */
typedef struct game {
    // the application config
    application_config config;

    // function pointer to game's initialize function
    b8 (*initialize)(struct game* game_inst);

    // function pointer to game's update function
    b8 (*update)(struct game* game_inst, f32 delta_type);

    // function pointer to game's render function
    b8 (*render)(struct game* game_inst, f32 delta_type);

    // function pointer to handle resize, if applicable
    void (*on_resize)(struct game* game_inst, u32 width, u32 height);

    // game specific state. created and managed by the game
    void* state;
} game;