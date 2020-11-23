#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define PORT 8080
int main (int argc, char *argv[]) {
    int cl_socket;
    struct sockaddr_in server_addr;

    if ((cl_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket: ");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, '\0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton (AF_INET, argv[1], &server_addr.sin_addr) < 0) {
        perror("inet_pton: ");
        close(cl_socket);
        exit(EXIT_FAILURE);
    }

    printf ("------Connecting to the server---------------\n");
    int r = connect(cl_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (r < 0) {
        perror("connect: ");
        close(cl_socket);
        exit(EXIT_FAILURE);
    }

    printf ("---------Connected successfully--------------\n"
            "-------------Writing to the server-----------\n");

    char send_buff[100] = {0};

    while (1) {

        memset(send_buff, '\0', sizeof(send_buff));

        int rcv_from_usr = read(STDIN_FILENO, send_buff, 99);

        if (rcv_from_usr < 0) {
            perror("read from stdin: ");
            close(cl_socket);
            exit(EXIT_FAILURE);
        }

        send_buff[rcv_from_usr - 1] = '\0';

        if (strcmp(send_buff, "Stop") == 0) {
            if (write(cl_socket, send_buff, strlen(send_buff)) < 0) {
                perror("write: ");
                close(cl_socket);
                exit(EXIT_FAILURE);
            }
            break;
        }

        if (write(cl_socket, send_buff, strlen(send_buff)) < 0) {
            perror("write: ");
            close(cl_socket);
            exit(EXIT_FAILURE);
        }



        printf("-----------Waiting the answer from the server-------------------\n");

        char rcv_buff[100] = {0};
        int read_ret_val = read(cl_socket, rcv_buff, 100);

        printf("From server: %d bytes: [%s]\n", read_ret_val, rcv_buff);
    }

    close(cl_socket);
    return 0;
}