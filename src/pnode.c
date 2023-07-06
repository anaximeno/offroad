#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
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
        while (true)
        {
            n = recv(socketfd, buffer, OFFROAD_BUFFER_LENGHT, 0);

            if (n <= 0)
                break;

            fprintf(file, "%s", buffer);

            memset(buffer, 0, OFFROAD_BUFFER_LENGHT);
        }

        fclose(file);
        return create_result(0, NULL, INFO);
    }
    else
    {
        return create_result(1, "Could not transfer the file from the client", ERROR);
    }
}

extern offroad_func_result *execute_pnode(struct pnode_args *args)
{
    struct sockaddr_in client;
    int socketfd, connection, bind_status, connection_status;
    socklen_t length;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketfd == -1)
        return create_result(1, "Could not create the socket", ERROR);

    struct sockaddr_in serveraddr = {
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(args->port),
        .sin_family = AF_INET};

    memset(&serveraddr, 0, sizeof(serveraddr));

    bind_status = bind(socketfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if (bind_status == -1)
    {
        close(socketfd);
        return create_result(1, "Could not bind the socket to the server address", ERROR);
    }

    connection_status = listen(socketfd, 5);

    if (connection_status == -1)
    {
        close(socketfd);
        return create_result(1, "Could not listen to the socket connection", ERROR);
    }

    length = sizeof(client);
    connection = accept(socketfd, (struct sockaddr *)&client, &length);

    if (connection == -1)
    {
        close(socketfd);
        return create_result(1, "Connection with the client could not be stablished", ERROR);
    }

    offroad_func_result *res = write_file(connection);

    close(socketfd);
    return res;
}
