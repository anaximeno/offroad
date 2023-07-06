#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../include/pnode.h"
#include "../include/common.h"

extern void free_pnode_args(struct pnode_args **args)
{
    free(*args);
    *args = NULL;
}

offroad_func_result *write_file(int socketfd)
{
    char buffer[OFFROAD_BUFFER_LENGHT] = {0};
    int n;

    FILE *file = fopen(OFFROAD_P_NODE_TMP_FILE, "w");

    if (file != NULL)
    {
        debug_msg("Receiving the file...", INFO);

        while (true)
        {
            n = recv(socketfd, buffer, OFFROAD_BUFFER_LENGHT, 0);

            if (n <= 0)
                break;

            fprintf(file, "%s", buffer);

            memset(buffer, 0, OFFROAD_BUFFER_LENGHT);
        }

        fclose(file);
        debug_msg("File received", INFO);
        return ok_result(NULL);
    }
    else
    {
        return err_result(1, "Could not transfer the file from the client", ERROR);
    }
}

offroad_func_result *execute_file(const char *filename)
{
    struct stat fs;

    if (stat(filename, &fs) == -1)
        return err_result(errno, "Couldn't get the status of the file", ERROR);
    else if (!S_ISREG(fs.st_mode))
        return err_result(1, "File can't be handled regularly", ERROR);
    else if (chmod(filename, S_IRWXU | S_IROTH) == -1)
        return err_result(errno, "Can't set execute permission on the run file", ERROR);

    // debug_msg("Executing the file...", INFO); // TODO

    // execv(filename) // TODO

    return ok_result(NULL);
}

extern offroad_func_result *execute_pnode(struct pnode_args *args)
{
    offroad_func_result *res = NULL;

    struct sockaddr_in client, serveraddr;
    int socketfd, connection, bind_status, connection_status;
    socklen_t length;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    debug_msg("Initializing the socket...", INFO);

    if (socketfd == -1)
    {
        res = err_result(errno, "Could not create the socket", ERROR);
        goto return_block;
    }

    memset(&client, 0, sizeof(client));
    memset(&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(args->port);
    serveraddr.sin_family = AF_INET;

    bind_status = bind(socketfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if (bind_status == -1)
    {
        res = err_result(errno, "Could not bind the socket to the server address", ERROR);
        goto return_block;
    }

    debug_msg("Socket successfully created, waiting for connections...", INFO);

    connection_status = listen(socketfd, 10);

    if (connection_status == -1)
    {
        res = err_result(errno, "Could not listen to the socket", ERROR);
        goto return_block;
    }

    length = sizeof(client);
    connection = accept(socketfd, (struct sockaddr *)&client, &length);

    if (connection == -1)
    {
        res = err_result(errno, "Connection with the client could not be stablished", ERROR);
        goto return_block;
    }

    debug_msg("Connection accepted", INFO);

    res = write_file(connection);

    if (res != NULL && res->type == OK)
    {
        free_result(&res);
        res = execute_file(OFFROAD_P_NODE_TMP_FILE);
    }

return_block:
    close(socketfd);
    return res;
}
