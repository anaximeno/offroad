#ifndef _OFFROAD
#define _OFFROAD

#include "argparser.h"
#include "common.h"

#include "ax-c-common/include/ax-c-common.h"

/** Executes a file in a remote processor. */
ax_result_p execute_offroad(offroad_cli_args *args);

#endif