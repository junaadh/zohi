#pragma once

#include "defines.h"

struct game;

// app config
typedef struct application_config {
    // window starting pos x axis
    i16 start_pos_x;

    // window starting pos y axis
    i16 start_pos_y;

    // window start width
    i16 start_width;

    // window start height
    i16 start_height;

    // app name
    char *name;
} application_config;

ZAPI b8 application_create(struct game *game_inst);
ZAPI b8 application_run();
