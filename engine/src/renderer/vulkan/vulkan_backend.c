#include "vulkan_backend.h"
#include "vulkan_platform.h"
#include "vulkan_types.inl"

#include "containers/darray.h"
#include "core/logger.h"
#include "core/zstring.h"

// static vulkan context
static vulkan_context context;

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data);

b8 vulkan_renderer_backend_initialize(renderer_backend *backend, const char *application_name, struct platform_state *plat_state) {
    // TODO: use custom allocator
    context.allocator = 0;

    // setup vulkan instance
    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = application_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Zohi Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;

    // obtain a list of required extension
    const char **required_extensions = darray_create(const char *);
    darray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME); // generic extensions
    platform_get_required_extension_names(&required_extensions);      // platform specific extensions
#if defined(_DEBUG)
    darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    ZDEBUG("Required extensions: ");
    u32 len = darray_length(required_extensions);
    for (u32 i = 0; i < len; ++i) {
        ZDEBUG(required_extensions[i]);
    }
#endif

    create_info.enabledExtensionCount = darray_length(required_extensions);
    create_info.ppEnabledExtensionNames = required_extensions;

    // validataion layers
    const char **required_validation_layer_names = 0;
    u32 required_validation_layer_count = 0;

// if validation should be done get a list of the required validation layer names
// and make sure they exist. validation layers should only be enabled on non-release builds
#if defined(_DEBUG)
    ZINFO("Validation layers enabled. Enumerating...");

    // the list of validation layers required
    required_validation_layer_names = darray_create(const char *);
    darray_push(required_validation_layer_names, &"VK_LAYER_KHRONOS_validation");
    required_validation_layer_count = darray_length(required_validation_layer_names);

    // obtain a list of available validation layers
    u32 available_layer_count = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
    VkLayerProperties *available_layers = darray_reserve(VkLayerProperties, available_layer_count);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers));

    // verify all required layers are available
    for (u32 i = 0; i < required_validation_layer_count; ++i) {
        ZINFO("Searching for layer: %s...", required_validation_layer_names[i]);
        b8 found = FALSE;
        for (u32 j = 0; j < available_layer_count; ++j) {
            if (strings_equal(required_validation_layer_names[i], available_layers[j].layerName)) {
                found = TRUE;
                ZINFO("found");
                break;
            }
        }

        if (!found) {
            ZFATAL("required validation layer is missing: %s", required_validation_layer_names[i]);
            return FALSE;
        }
    }
    ZINFO("All required validation layers are present.");
#endif

    create_info.enabledLayerCount = required_validation_layer_count;
    create_info.ppEnabledLayerNames = required_validation_layer_names;

    VK_CHECK(vkCreateInstance(&create_info, context.allocator, &context.instance));
    ZINFO("Vulkan Instance created.");

#if defined(_DEBUG)
    ZDEBUG("Creating Vulkan debugger...");
    u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT; //|
                                                                     //    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity = log_severity;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;
    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
    ZASSERT_MSG(func, "Failed to create debug messenger!");
    VK_CHECK(func(context.instance, &debug_create_info, context.allocator, &context.debug_messenger));
    ZDEBUG("Vulkan debugger created.");
#endif

    ZINFO("Vulkan renderer initialized successfully.");
    return TRUE;
}

void vulkan_renderer_backend_shutdown(renderer_backend *backend) {
    ZDEBUG("Destroying Vulkan debugger...");
    if (context.debug_messenger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debug_messenger, context.allocator);
    }
    ZDEBUG("Destroying Vulkan instance...");
    vkDestroyInstance(context.instance, context.allocator);
}

void vulkan_renderer_backend_on_resized(renderer_backend *backend, u16 width, u16 height) {
}

b8 vulkan_renderer_backend_begin_frame(renderer_backend *backend, f32 delta_time) {
    return TRUE;
}

b8 vulkan_renderer_backend_end_frame(renderer_backend *backend, f32 delta_time) {
    return TRUE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data) {
    switch (message_severity) {
    default:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        ZERROR(callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        ZWARN(callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        ZINFO(callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        ZTRACE(callback_data->pMessage);
        break;
    }
    return VK_FALSE;
}
