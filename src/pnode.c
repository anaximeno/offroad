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
#include "../include/offroad.h"

#include "../ax-c-common/ax-c-common.h"

extern void free_pnode_args(struct pnode_args **args)
{
    free(*args);
    *args = NULL;
}

ax_result(NULL) write_file(int socketfd)
{
    char buffer[OFFROAD_BUFFER_LENGHT] = {0};
    int n;

    FILE *file = fopen(OFFROAD_P_NODE_TMP_FILE, "w");

    if (file != NULL)
    {
        ax_log(INFO, "Receiving the file...");

        while (true)
        {
            n = recv(socketfd, buffer, OFFROAD_BUFFER_LENGHT, 0);

            if (n <= 0)
                break;

            fprintf(file, "%s", buffer);

            memset(buffer, 0, OFFROAD_BUFFER_LENGHT);
        }

        fclose(file);
        ax_log(INFO, "File received");
        return ax_result_ok(NULL);
    }
    else
    {
        return ax_result_err(1, "Could not transfer the file from the client");
    }
}

ax_result(NULL) execute_file(const char *filename)
{
    struct stat fs;

    if (stat(filename, &fs) == -1)
        return ax_result_err(errno, "Couldn't get the status of the file");
    else if (!S_ISREG(fs.st_mode))
        return ax_result_err(1, "File can't be handled regularly");
    else if (chmod(filename, S_IRWXU | S_IROTH) == -1)
        return ax_result_err(errno, "Can't set execute permission on the run file");

    char *path = axcallocate(strlen(filename) + 2, char);
    char *log_info = axcallocate(strlen(filename) + 64, char);

    sprintf(path, "./%s", filename);
    sprintf(log_info, "Running received file at './%s' ...\n", filename);
    ax_log(INFO, log_info);
    axfree(log_info);

    int result = execl(path, filename, NULL);
    axfree(path);

    if (result == -1)
        return ax_result_err(1, "Error while trying to running the file");

    return ax_result_ok(NULL);
}

extern ax_result(NULL) execute_pnode(struct pnode_args *args)
{
    struct sockaddr_in client, serveraddr;
    int socketfd, connection, bind_status, connection_status;
    socklen_t length;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    ax_log(INFO, "Initializing the socket...");

    if (socketfd == -1)
        return ax_result_err(errno, "Could not create the socket");

    memset(&client, 0, sizeof(client));
    memset(&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(args->port);
    serveraddr.sin_family = AF_INET;

    bind_status = bind(socketfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if (bind_status == -1)
    {
        close(socketfd);
        return ax_result_err(errno, "Could not bind the socket to the server address");
    }

    char message[OFFROAD_BUFFER_LENGHT] = {0};

    sprintf(message, "Socket successfully created at the port = %d", args->port);
    ax_log(INFO, message);

    connection_status = listen(socketfd, 10);

    if (connection_status == -1)
    {
        close(socketfd);
        return ax_result_err(errno, "Could not listen to the socket");
    }

    length = sizeof(client);
    connection = accept(socketfd, (struct sockaddr *)&client, &length);

    if (connection == -1)
    {
        close(socketfd);
        return ax_result_err(errno, "Connection with the client could not be stablished");
    }

    ax_log(INFO, "Connection accepted");

    ax_result_p result = write_file(connection);

    if (result != NULL && result->type == OK)
    {
        ax_free_result(&result);
        result = execute_file(OFFROAD_P_NODE_TMP_FILE);
    }

    return result;
}
