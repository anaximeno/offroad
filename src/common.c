#include <stdio.h>
#include <stdlib.h>

#include "../include/common.h"

char* debug_msg_prefix(offroad_debug_level level)
{
    switch (level)
    {
    case INFO:
        return "info";
    case WARNING:
        return "warning";
    case ERROR:
        return "error";
    case CRITICAL:
        return "critical";
    default:
        return "debug message";
    }
}

extern void debug_msg(const char* message, offroad_debug_level level)
{
    char* pfix = debug_msg_prefix(level);
    fprintf(stderr, "offroad: %s: %s\n", pfix, message);

    if (level == CRITICAL)
    {
        exit(1);
    }
}