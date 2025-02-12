#include "core/input.h"
#include "core/event.h"
#include "core/logger.h"
#include "core/zmemory.h"
#include "defines.h"

typedef struct keyboard_state {
    b8 keys[256];
} keyboard_state;

typedef struct mouse_state {
    i16 x;
    i16 y;
    u8 buttons[BUTTON_MAX_BUTTONS];
} mouse_state;

typedef struct input_state {
    keyboard_state keyboard_current;
    keyboard_state keyboard_previous;
    mouse_state mouse_current;
    mouse_state mouse_previous;
} input_state;

// internal input state
static b8 initialized = FALSE;
static input_state state = {};

void input_initialize() {
    zzero_memory(&state, sizeof(input_state));
    initialized = TRUE;
    ZINFO("input subsystem initialized");
}

void input_shutdown() {
    // TODO:vadd shutdown routines when needed
    initialized = FALSE;
}

void input_update(f64 delta_time) {
    if (!initialized)
        return;

    // copy current states to previous states
    zcopy_memory(&state.keyboard_previous, &state.keyboard_current, sizeof(keyboard_state));
    zcopy_memory(&state.mouse_previous, &state.keyboard_current, sizeof(mouse_state));
}

void input_process_key(keys key, b8 pressed) {
    // only handle this if the state actually changed
    if (state.keyboard_current.keys[key] != pressed) {
        // update internal state
        state.keyboard_current.keys[key] = pressed;

        // fire off an event for immediate processing
        event_context context;
        context.data.u16[0] = key;
        event_fire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    }
}

void input_process_button(buttons button, b8 pressed) {
    // if the state changed, fire an event
    if (state.mouse_current.buttons[button] != pressed) {
        state.mouse_current.buttons[button] = pressed;

        // fire the event
        event_context context;
        context.data.u16[0] = button;
        event_fire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}

void input_process_mouse_move(i16 x, i16 y) {
    // only process if actually different
    if (state.mouse_current.x != x || state.mouse_current.y != y) {
        // NOTE: enable this if debugging
        // ZDEBUG("Mouse pos: %i, %i!", x, y);

        // update internal state
        state.mouse_current.x = x;
        state.mouse_current.y = y;

        // fire the event
        event_context context;
        context.data.u16[0] = x;
        context.data.u16[1] = y;
        event_fire(EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}

void input_process_mouse_wheel(i8 z_delta) {
    // NOTE: no internal state to update

    // fire the event
    event_context context;
    context.data.u8[0] = z_delta;
    event_fire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}

// keyboard
b8 input_is_key_down(keys key) {
    if (!initialized)
        return FALSE;

    return state.keyboard_current.keys[key] == TRUE;
}

b8 input_is_key_up(keys key) {
    if (!initialized)
        return FALSE;

    return state.keyboard_current.keys[key] == FALSE;
}

b8 input_was_key_down(keys key) {
    if (!initialized)
        return FALSE;

    return state.keyboard_previous.keys[key] == TRUE;
}

b8 input_was_key_up(keys key) {
    if (!initialized)
        return FALSE;

    return state.keyboard_previous.keys[key] == FALSE;
}

// mouse
b8 input_is_button_down(buttons button) {
    if (!initialized)
        return FALSE;

    return state.mouse_current.buttons[button] == TRUE;
}

b8 input_is_button_up(buttons button) {
    if (!initialized)
        return FALSE;

    return state.mouse_current.buttons[button] == FALSE;
}

b8 input_was_button_down(buttons button) {
    if (!initialized)
        return FALSE;

    return state.mouse_previous.buttons[button] == TRUE;
}

b8 input_was_button_up(buttons button) {
    if (!initialized)
        return FALSE;

    return state.mouse_previous.buttons[button] == FALSE;
}

void input_get_mouse_position(i32 *x, i32 *y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = state.mouse_current.x;
    *y = state.mouse_current.y;
}

void input_get_previous_mouse_position(i32 *x, i32 *y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = state.mouse_previous.x;
    *y = state.mouse_previous.y;
}
