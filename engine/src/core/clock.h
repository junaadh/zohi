#pragma once
#include "defines.h"

typedef struct clock {
    f64 start_time;
    f64 elapsed;
} clock;

// updates the clock. should be called before checking elapsed time
// has no effect on non started clocks
void clock_update(clock* clock);

// starts the provided clock
void clock_start(clock* clock);

// stops the provided clock
void clock_stop(clock* clock);
