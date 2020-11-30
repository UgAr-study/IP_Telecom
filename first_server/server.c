#include "header.h"

void handler (int signo) {
    if (signo == SIGCHLD) {
        int status = 0;
        wait (&status);

        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == EXIT_SUCCESS)
                printf("child successfully exited\n");
        } else
            printf("child was killed\n");
    } else
        printf ("Unknow signal\n");
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);


    if (bind(server_fd, (struct sockaddr *)&address, sizeof address)<0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, NULL) < 0) {
        perror ("sigaction: ");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    int num_connections = 0;
    while(1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        if (!fork()) {
            close(server_fd);

            printf ("Connected with %u\n", address.sin_addr.s_addr);

            char buffer[100] = {0};
            char send_mes[100] = {0};

            while (1) {
                memset(buffer, '\0', strlen(buffer));
                valread = read(new_socket, buffer, 100);

                if (valread < 0) {
                    printf("error with read\n");
                    close(new_socket);
                    exit(EXIT_FAILURE);
                }

                if (valread == 0) {
                    printf ("nothing to read\n");
                    close(new_socket);
                    exit(EXIT_SUCCESS);
                }

                if (strcmp(buffer, "Stop") == 0) {
                    printf("finish\n");
                    close(new_socket);
                    exit(EXIT_SUCCESS);
                }

                printf("From client: %ld bytes: [%s]\n", valread, buffer);
                fflush(stdout);

                int r_from_usr = read(STDIN_FILENO, send_mes, 100);
                send_mes[r_from_usr - 1] = '\0';

                write(new_socket, send_mes, strlen(send_mes));
                printf("-----------------Message sent-------------------\n");
            }

            exit(EXIT_SUCCESS);
        }

        close(new_socket);
        num_connections++;

        if (num_connections == 2)
            break;
    }

    close(new_socket);
    close (server_fd);

    return 0;
}
