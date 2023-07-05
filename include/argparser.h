#ifndef _OFFROAD_ARGPARSER
#define _OFFROAD_ARGPARSER

/** Defines the spected arguments for the program. */
typedef struct offroad_cli_args {
    const char* filename;
    const char* error;
} offroad_cli_args;


/** Returns the command line arguments for the program. */
offroad_cli_args* parse_args(int argc, char** argv);

/** Securely frees the args variable. */
void free_args(offroad_cli_args** args);

#endif