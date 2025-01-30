#pragma once

#include "defines.h"

#include "core/asserts.h"
#include <vulkan/vulkan.h>

// checks the given expression's return value against VK_SUCCESS
#define VK_CHECK(expr) \
    ZASSERT(expr == VK_SUCCESS);

typedef struct vulkan_context {
    VkInstance instance;
    VkAllocationCallbacks *allocator;
#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} vulkan_context;
