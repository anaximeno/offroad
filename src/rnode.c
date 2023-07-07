#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../include/rnode.h"
#include "../include/offroad.h"

#include "../include/ax-c-common/include/ax-c-common.h"

extern void free_rnode_args(struct rnode_args **args)
{
    if ((*args)->file != NULL)
        fclose((*args)->file);

    free(*args);
    *args = NULL;
}

ax_result_p send_file(FILE *file, int socketfd)
{
    char data[OFFROAD_BUFFER_LENGHT] = {0};

    while (fgets(data, OFFROAD_BUFFER_LENGHT, file) != NULL)
    {
        if (send(socketfd, data, sizeof(data), 0) == -1)
            return ax_result_err(1, "Error while sending the file");
        memset(data, 0, OFFROAD_BUFFER_LENGHT);
    }

    return ax_result_ok(NULL);
}

extern ax_result_p execute_rnode(struct rnode_args *args)
{
    struct sockaddr_in client; // XXX
    int socketfd, connection;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketfd == -1)
        return ax_result_err(1, "Error during the creation of the socket");

    struct sockaddr_in serveraddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(args->host),
        .sin_port = htons(args->port)};

    connection = connect(socketfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if (connection == -1)
    {
        close(socketfd);
        return ax_result_err(1, "Could not create a connection to the server");
    }

    ax_result_p result = send_file(args->file, socketfd);

    close(socketfd);
    return result;
}