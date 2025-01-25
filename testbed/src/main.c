#include <core/logger.h>
#include <core/asserts.h>
#include <platform/platform.h>

int main(void) {
    ZFATAL("HELLO, %f", 6.9f);
    ZERROR("HELLO, %f", 6.9f);
    ZWARN("HELLO, %f", 6.9f);
    ZINFO("HELLO, %f", 6.9f);
    ZDEBUG("HELLO, %f", 6.9f);
    ZTRACE("HELLO, %f", 6.9f);

    platform_state state;
    if (platform_startup(&state, "Zohi Testbed", 100, 100, 1200, 720)) {
        while (TRUE) {
            platform_pump_messages(&state);
        }
    }
    platform_shutdown(&state);
    
    return 0;
}