#ifndef _OFFROAD_COMMON
#define _OFFROAD_COMMON

typedef enum offroad_debug_level
{
    INFO,
    WARNING,
    ERROR,
    CRITICAL
} offroad_debug_level;

void debug_msg(const char* message, offroad_debug_level level);

#endif