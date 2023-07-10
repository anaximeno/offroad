#ifndef _OFFROAD
#define _OFFROAD

#include "argparser.h"

#include "../ax-c-common/ax-c-common.h"

#define OFFROAD_BUFFER_LENGHT 1024
#define OFFROAD_DEFAULT_HOST "127.0.0.1"
#define OFFROAD_DEFAULT_PORT 9091

typedef enum offroad_platform_type
{
    LINUX,
    WINDOWS,
    MACOS,
    FREEBSD
} offroad_platform_type;

/** Executes a file in a remote processor. */
ax_result_p execute_offroad(offroad_cli_args *args);

#endif