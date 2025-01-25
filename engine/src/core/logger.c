#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"

// TODO: temp
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void report_assertion_failure(const char *expression, const char *msg, const char *file, i32 line) {
    log_output(LOG_LEVLEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, msg, file, line);
}

b8 initialize_logging() {
    // TODO: create log file
    return TRUE;
}

void shutdown_logging() {
    // TODO: cleanup logging/write queued entries
}

void log_output(log_level level, const char *msg, ...) {
    const char *lvls[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    b8 is_error = level < LOG_LEVEL_WARN;

    // create a buffer of 32k characters
    // DONT DO THIS
    const i32 msg_len = 32000;
    char out_msg[msg_len];
    memset(out_msg, 0, sizeof(out_msg));

    // format original msg
    // NOTE: MS's headers override gcc/clang va_list type whit a typedef char* va_list in some case
    // therefore the workaround is to use __builtin_va_list which is the type gcc/clang's va_start expects
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(out_msg, msg_len, msg, arg_ptr);
    va_end(arg_ptr);

    char final[msg_len];
    sprintf(final, "%s%s\n", lvls[level], out_msg);

    // platform specific output
    if (is_error) {
        platform_console_write_error(final, level);
    } else {
        platform_console_write(final, level);
    }
}