#ifndef _OFFROAD_COMMON
#define _OFFROAD_COMMON

#define OFFROAD_BUFFER_LENGHT 1024
#define OFFROAD_DEFAULT_HOST "127.0.0.1"
#define OFFROAD_DEFAULT_PORT 9091

typedef enum offroad_debug_level
{
    INFO,
    WARNING,
    ERROR,
    CRITICAL
} offroad_debug_level;

typedef enum offroad_run_type
{
    NONE,
    PNODE,
    RNODE
} offroad_run_type;

typedef enum offroad_platform_type
{
    LINUX,
    WINDOWS,
    MACOS,
    FREEBSD
} offroad_platform_type;

typedef struct offroad_func_result
{
    int status;
    char *error;
    offroad_debug_level dbg_level;
} offroad_func_result;

void debug_msg(const char *message, offroad_debug_level level);

offroad_func_result* create_result(int status, char* error, offroad_debug_level level);

#endif