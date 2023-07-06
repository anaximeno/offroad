#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../include/rnode.h"
#include "../include/common.h"

extern void free_rnode_args(struct rnode_args **args)
{
    if ((*args)->file != NULL)
        fclose((*args)->file);

    free(*args);
    *args = NULL;
}

offroad_func_result *send_file(FILE *file, int socketfd)
{
    char data[OFFROAD_BUFFER_LENGHT] = {0};

    while (fgets(data, OFFROAD_BUFFER_LENGHT, file) != NULL)
    {
        if (send(socketfd, data, sizeof(data), 0) == -1)
            return create_result(1, "Error while sending the file", ERROR);

        bzero(data, OFFROAD_BUFFER_LENGHT);
    }

    return create_result(0, NULL, INFO);
}

extern offroad_func_result *execute_rnode(struct rnode_args *args)
{
    struct sockaddr_in serveraddr, client;
    int socketfd, connection;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketfd == -1)
        return create_result(1, "Error during the creation of the socket", ERROR);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(args->host);
    serveraddr.sin_port = htons(args->port);

    connection = connect(socketfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if (connection == -1)
    {
        close(socketfd);
        return create_result(1, "Could not create a connection to the server", ERROR);
    }

    offroad_func_result *res = send_file(args->file, socketfd);

    close(socketfd);
    return res;
}