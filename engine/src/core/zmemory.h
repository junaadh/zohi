#pragma once

#include "defines.h"

typedef enum memory_tag {
    // for temporary use. should be assigned one of the below or have a new tag created
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,

    MEMORY_TAG_MAX_TAGS
} memory_tag;

ZAPI void initialize_memory();
ZAPI void shutdown_memory();

ZAPI void* zallocate(u64 size, memory_tag tag);
ZAPI void zfree(void* block, u64 size, memory_tag tag);
ZAPI void* zzero_memory(void* block, u64 size);
ZAPI void* zcopy_memory(void* block, const void* source, u64 size);
ZAPI void* set_memory(void* block, i32 value, u64 size);
ZAPI char* get_memory_usage_str();