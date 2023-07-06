#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/common.h"

static char *__program__name__ = "offroad";

void __set_program_name(char *name)
{
    __program__name__ = name;
}

char *__get_program_name()
{
    return __program__name__;
}

char *debug_msg_prefix(offroad_debug_level level)
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

extern void debug_msg(const char *msg, offroad_debug_level level)
{
    if (level == NONE)
        return;

    char *pfix = debug_msg_prefix(level);

    fprintf(stderr, "%s: %s: %s\n", __program__name__, pfix, msg);

    if (level == CRITICAL)
        exit(1);
}

extern offroad_func_result *err_result(int status, char *message, offroad_debug_level level)
{
    offroad_func_result *res = (offroad_func_result *)malloc(sizeof(offroad_func_result));

    if (res != NULL)
    {
        res->type = ERR;
        res->to.err.dbg_level = level;
        res->to.err.status = status;
        res->to.err.message = message;
    }

    return res;
}

extern offroad_func_result *ok_result(void *value)
{
    offroad_func_result *res = (offroad_func_result *)malloc(sizeof(offroad_func_result));

    if (res != NULL)
    {
        res->type = OK;
        res->to.ok.value = value;
    }

    return res;
}

extern void free_result(offroad_func_result **result)
{
    if ((*result)->type == OK && ((*result)->to.ok.value != NULL))
        free((*result)->to.ok.value);

    free(*result);
    *result = NULL;
}