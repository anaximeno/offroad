#ifndef _OFFROAD_COMMON
#define _OFFROAD_COMMON

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define OFFROAD_BUFFER_LENGHT 1024
#define OFFROAD_DEFAULT_HOST "127.0.0.1"
#define OFFROAD_DEFAULT_PORT 9091

void __set_program_name(char *name);
char *__get_program_name();

#define SET_PROG_NAME(name) __set_program_name(name)
#define GET_PROG_NAME() __get_program_name()

typedef enum offroad_run_type
{
    UNKNOWN,
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

#endif