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

extern void debug_msg(const char* msg, offroad_debug_level level)
{
    char* pfix = debug_msg_prefix(level);

    fprintf(level != INFO ? stderr : stdout, "offroad: %s: %s\n", pfix, msg);

    if (level == CRITICAL)
    {
        exit(1);
    }
}