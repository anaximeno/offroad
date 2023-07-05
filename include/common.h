#ifndef _OFFROAD_COMMON
#define _OFFROAD_COMMON

typedef enum offroad_debug_level
{
    INFO,
    WARNING,
    ERROR,
    CRITICAL
} offroad_debug_level;

typedef enum offroad_run_type
{
    PNODE,
    RNODE
} offroad_run_type;

void debug_msg(const char *message, offroad_debug_level level);

#endif