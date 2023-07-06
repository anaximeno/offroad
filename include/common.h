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

typedef enum offroad_debug_level
{
    NONE,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
} offroad_debug_level;

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

enum offroad_func_result_type
{
    OK,
    ERR
};

struct offroad_func_result_err
{
    char *message;
    offroad_debug_level dbg_level;
    int status;
};

struct offroad_func_result_ok
{
    void *value;
};

typedef struct offroad_func_result
{
    enum offroad_func_result_type type;

    union concrete_result
    {
        struct offroad_func_result_ok ok;
        struct offroad_func_result_err err;
    } to;
} offroad_func_result;

/** Print debug message. */
void debug_msg(const char *message, offroad_debug_level level);

/** Result of a insuccessfully executed function or procedure. */
offroad_func_result *err_result(int status, char *message, offroad_debug_level level);

/** Result of a successfully executed function or procedure. */
offroad_func_result *ok_result(void *value);

void free_result(offroad_func_result **result);

#define UNWRAP_OK_RESULT(result, dest, type)                       \
    memmove((dest), (type *)((result)->to.ok.value), sizeof(type)); \
    free((result)->to.ok.value);                                   \
    (result)->to.ok.value = NULL;                                  \
    free_result(&(result));

#endif