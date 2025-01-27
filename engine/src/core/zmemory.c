#include "zmemory.h"

#include "core/logger.h"
#include "platform/platform.h"
#include "core/zstring.h"

#include <stdio.h>
#include <string.h>

struct memory_stat {
    u64 total_allocations;
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static struct memory_stat stats;

static const char *memory_tag_string[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      "};

void initialize_memory() {
    platform_zero_mem(&stats, sizeof(stats));
}

void shutdown_memory() {
}

void *zallocate(u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        ZWARN("zallocate called using MEMORY_TAG_UNKNOWN. Re-Class this allocation");
    }

    stats.total_allocations += size;
    stats.tagged_allocations[tag] += size;

    // TODO: memory alignment
    void *block = platform_allocate(size, FALSE);
    platform_zero_mem(block, size);
    return block;
}

void zfree(void *block, u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        ZWARN("zallocate called using MEMORY_TAG_UNKNOWN. Re-Class this allocation");
    }

    stats.total_allocations -= size;
    stats.tagged_allocations[tag] -= size;

    // TODO: memory alignment
    platform_allocate(size, FALSE);
}

void *zzero_memory(void *block, u64 size) {
    return platform_zero_mem(block, size);
}

void *zcopy_memory(void *block, const void *source, u64 size) {
    return platform_copy_mem(block, source, size);
}

void *zset_memory(void *block, i32 value, u64 size) {
    return platform_set_mem(block, value, size);
}

char *get_memory_usage_str() {
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "System memory usage (tagged):\n";
    u64 offset = strlen(buffer);

    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "XiB";
        float amount = 1.0f;

        if (stats.tagged_allocations[i] >= gib) {
            unit[0] = 'G';
            amount = stats.tagged_allocations[i] / (float)gib;
        } else if (stats.tagged_allocations[i] >= mib) {
            unit[0] = 'M';
            amount = stats.tagged_allocations[i] / (float)mib;
        } else if (stats.tagged_allocations[i] >= kib) {
            unit[0] = 'K';
            amount = stats.tagged_allocations[i] / (float)kib;
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float)stats.tagged_allocations[i];
        }

        i32 length = snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", memory_tag_string[i], amount, unit);
        offset += length;
    }

    char *out_string = string_duplicate(buffer);
    return out_string;
}
