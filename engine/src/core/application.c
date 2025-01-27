#include "application.h"
#include "core/zmemory.h"
#include "game_types.h"
#include "logger.h"
#include "platform/platform.h"

typedef struct application_state {
    game *game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;

b8 application_create(game *game_inst) {
    if (initialized) {
        ZERROR("application_create called more than once");
        return FALSE;
    }

    app_state.game_inst = game_inst;

    // init subsystems
    initialize_logging();

    // TODO: remove this
    ZFATAL("HELLO, %f", 6.9f);
    ZERROR("HELLO, %f", 6.9f);
    ZWARN("HELLO, %f", 6.9f);
    ZINFO("HELLO, %f", 6.9f);
    ZDEBUG("HELLO, %f", 6.9f);
    ZTRACE("HELLO, %f", 6.9f);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if (!platform_startup(
            &app_state.platform,
            game_inst->config.name,
            game_inst->config.start_pos_x,
            game_inst->config.start_pos_y,
            game_inst->config.start_width,
            game_inst->config.start_height)) {
        return FALSE;
    }

    // init the game
    if (!app_state.game_inst->initialize(app_state.game_inst)) {
        ZFATAL("Game failed to initialize");
        return FALSE;
    }

    app_state.game_inst->on_resize(app_state.game_inst, app_state.width, app_state.height);
    initialized = TRUE;

    return TRUE;
}

b8 application_run() {
    ZINFO(get_memory_usage_str());

    while (app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            if (!app_state.game_inst->update(app_state.game_inst, (f32)0)) {
                ZFATAL("Game update failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_inst->render(app_state.game_inst, (f32)0)) {
                ZFATAL("Game render failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }
        }
    }

    app_state.is_running = FALSE;
    platform_shutdown(&app_state.platform);

    return TRUE;
}
