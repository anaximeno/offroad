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

ax_result(NULL) send_file(FILE *file, int socketfd)
{
    char data[OFFROAD_BUFFER_LENGHT] = {0};

    ax_log(INFO, "Sending the file to the server...");
    while (fgets(data, OFFROAD_BUFFER_LENGHT, file) != NULL)
    {
        if (send(socketfd, data, sizeof(data), 0) == -1)
            return ax_result_err(1, "Error while sending the file");
        memset(data, 0, OFFROAD_BUFFER_LENGHT);
    }
    ax_log(INFO, "File sent.");

    return ax_result_ok(NULL);
}

extern ax_result(NULL) execute_rnode(struct rnode_args *args)
{
    // XXX: will be used for bidirectional
    // communication
    struct sockaddr_in client;
    int socketfd, connection;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    ax_log(INFO, "Creating a socket for the connection...");

    if (socketfd == -1)
        return ax_result_err(1, "Error during the creation of the socket");

    struct sockaddr_in serveraddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(args->host),
        .sin_port = htons(args->port)};


    ax_log(INFO, "Trying to connect to the server...");
    connection = connect(socketfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if (connection == -1)
    {
        close(socketfd);
        return ax_result_err(1, "Could not create a connection to the server");
    }

    ax_result(NULL) result = send_file(args->file, socketfd);

    close(socketfd);
    return result;
}