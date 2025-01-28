#include "application.h"
#include "core/clock.h"
#include "core/event.h"
#include "core/input.h"
#include "core/zmemory.h"
#include "defines.h"
#include "game_types.h"
#include "logger.h"
#include "platform/platform.h"
#include "renderer/renderer_frontend.h"

typedef struct application_state {
    game *game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    clock clock;
    f64 last_time;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;

// event handlers
b8 application_on_event(u16 code, void *sender, void *listener_inst, event_context context);
b8 application_on_key(u16 code, void *sender, void *listener_inst, event_context context);

b8 application_create(game *game_inst) {
    if (initialized) {
        ZERROR("application_create called more than once");
        return FALSE;
    }

    app_state.game_inst = game_inst;

    // init subsystems
    initialize_logging();
    input_initialize();

    // TODO: remove this
    ZFATAL("HELLO, %f", 6.9f);
    ZERROR("HELLO, %f", 6.9f);
    ZWARN("HELLO, %f", 6.9f);
    ZINFO("HELLO, %f", 6.9f);
    ZDEBUG("HELLO, %f", 6.9f);
    ZTRACE("HELLO, %f", 6.9f);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if (!event_initialize()) {
        ZERROR("event system failed initialization. application can continue");
        return FALSE;
    }

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    if (!platform_startup(
            &app_state.platform,
            game_inst->config.name,
            game_inst->config.start_pos_x,
            game_inst->config.start_pos_y,
            game_inst->config.start_width,
            game_inst->config.start_height)) {
        return FALSE;
    }

    // renderer startup
    if (!renderer_initialize(game_inst->config.name, &app_state.platform)) {
        ZFATAL("failed to initialize renderer. aborting application.");
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
    clock_stop(&app_state.clock);
    clock_update(&app_state.clock);
    app_state.last_time = app_state.clock.elapsed;

    f64 running_time = 0;
    u8 frame_count = 0;
    f64 target_frame_seconds = 1.0f / 60;

    ZINFO(get_memory_usage_str());

    while (app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            // update clock and get delta
            clock_update(&app_state.clock);
            f64 current_time = app_state.clock.elapsed;
            f64 delta = (current_time - app_state.last_time);
            f64 frame_start_time = platform_get_absolute_time();

            if (!app_state.game_inst->update(app_state.game_inst, (f32)delta)) {
                ZFATAL("Game update failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_inst->render(app_state.game_inst, (f32)delta)) {
                ZFATAL("Game render failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }

            // FIXME: refactor this
            render_packet packet;
            packet.delta_time = delta;
            renderer_draw_frame(&packet);

            f64 frame_end_time = platform_get_absolute_time();
            f64 frame_elapsed_time = frame_end_time - frame_start_time;
            running_time += frame_elapsed_time;
            f64 remaining_seconds = target_frame_seconds - frame_elapsed_time;

            if (remaining_seconds > 0) {
                u64 remaining_ms = (remaining_seconds * 1000);

                // if there is time left, give back to the os
                b8 limit_frames = FALSE;
                if (remaining_ms > 0 && limit_frames) {
                    platform_sleep(remaining_ms - 1);
                }

                frame_count++;
            }

            // NOTE: input update/state copying should always be handled
            // after any input should be recorded
            // As a safety input is the last to be updated before this frame ends
            input_update(delta);

            // update last time
            app_state.last_time = current_time;
        }
    }

    app_state.is_running = FALSE;

    // shutdown event system
    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    event_shutdown();
    input_shutdown();
    renderer_shutdown();

    platform_shutdown(&app_state.platform);

    return TRUE;
}

b8 application_on_event(u16 code, void *sender, void *listener_inst, event_context context) {
    switch (code) {
    case EVENT_CODE_APPLICATION_QUIT: {
        ZINFO("EVENT_CODE_APPLICATION_QUIT recieved, shutting down.\n");
        app_state.is_running = FALSE;
        return TRUE;
    }
    }
    return FALSE;
}

b8 application_on_key(u16 code, void *sender, void *listener_inst, event_context context) {
    if (code == EVENT_CODE_KEY_PRESSED) {
        u16 key_code = context.data.u16[0];
        if (key_code == KEY_ESCAPE) {
            // NOTE: techincally firing an event to itself, but there may be other listeners
            event_context data = {};
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);

            // block anything from processing this
            return TRUE;
        } else if (key_code == KEY_A) {
            // example on checking for a key
            ZDEBUG("Explicit: A key pressed");
        } else {
            ZDEBUG("'%c' key pressed in window", key_code);
        }
    } else if (code == EVENT_CODE_KEY_RELEASED) {
        u16 key_code = context.data.u16[0];
        if (key_code == KEY_B) {
            ZDEBUG("Explicit: B key released");
        } else {
            ZDEBUG("'%c' key released in window", key_code);
        }
    }
    return FALSE;
}
