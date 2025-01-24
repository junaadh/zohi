#include <core/logger.h>
#include <core/asserts.h>

int main(void) {
    KFATAL("FUCK, %f", 6.9f);
    KERROR("FUCK, %f", 6.9f);
    KWARN("FUCK, %f", 6.9f);
    KINFO("FUCK, %f", 6.9f);
    KDEBUG("FUCK, %f", 6.9f);
    KTRACE("FUCK, %f", 6.9f);

    KASSERT(1 == 69);
    return 0;
}