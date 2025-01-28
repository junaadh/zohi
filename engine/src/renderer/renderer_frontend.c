#include "renderer/renderer_frontend.h"
#include "defines.h"
#include "renderer/renderer_backend.h"

#include "core/logger.h"
#include "core/zmemory.h"

// backend render context
static renderer_backend *backend = 0;

b8 renderer_initialize(const char *application_name, struct platform_state *plat_state) {
    backend = zallocate(sizeof(renderer_backend), MEMORY_TAG_RENDERER);

    // TODO: make this configurable
    renderer_background_create(RENDERER_BACKEND_TYPE_VULKAN, plat_state, backend);
    backend->frame_number = 0;

    if (!backend->initialize(backend, application_name, plat_state)) {
        ZFATAL("renderer backend failed to initialize. shutting down...");
        return FALSE;
    }
    return TRUE;
}

void renderer_shutdown() {
    backend->shutdown(backend);
    zfree(backend, sizeof(renderer_backend), MEMORY_TAG_RENDERER);
}

void renderer_on_resized(u16 width, u16 height);

b8 renderer_begin_frame(f32 delta_time) {
    return backend->begin_frame(backend, delta_time);
}
b8 renderer_end_frame(f32 delta_time) {
    b8 result = backend->end_frame(backend, delta_time);
    backend->frame_number++;
    return result;
}

b8 renderer_draw_frame(render_packet *packet) {
    // if the begin frame returned success, mid-frame operations may continue
    if (renderer_begin_frame(packet->delta_time)) {

        // end the frame if this fails, it is likely unrecoverable
        b8 result = renderer_end_frame(packet->delta_time);
        if (!result) {
            ZERROR("renderer_end_frame failed. application shutting down...");
            return FALSE;
        }
    }

    return TRUE;
}
