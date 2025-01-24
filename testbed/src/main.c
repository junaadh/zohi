#include <core/logger.h>
#include <core/asserts.h>

int main(void) {
    ZFATAL("FUCK, %f", 6.9f);
    ZERROR("FUCK, %f", 6.9f);
    ZWARN("FUCK, %f", 6.9f);
    ZINFO("FUCK, %f", 6.9f);
    ZDEBUG("FUCK, %f", 6.9f);
    ZTRACE("FUCK, %f", 6.9f);

    ZASSERT(1 == 69);
    return 0;
}