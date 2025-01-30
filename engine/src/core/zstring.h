#pragma once

#include "defines.h"

ZAPI u64 string_length(const char *str);
ZAPI char *string_duplicate(const char *str);
ZAPI b8 strings_equal(const char* str0, const char* str1);
