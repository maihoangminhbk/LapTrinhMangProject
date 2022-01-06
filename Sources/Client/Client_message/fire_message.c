#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

#include <client_message.h>
#define BUFF_SIZE 1024

int fire_message(int client_sock, int turn)
{
    char fire_position[20];
    char buff[256];

    if (turn == 1)
    {
        int bytes_received;

        bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
        if (bytes_received < 0)
            perror("\nError: ");
        else if (bytes_received == 0)
            printf("Connection closed.\n");

        buff[bytes_received] = '\0';

        printf("Message nhan duoc la %s\n", buff);
    }

    while (1 == 1)
    {
        printf("Please input fire position");
        fgets(fire_position, 20, stdin);
        fire_position[strlen(fire_position) - 1] = '\0';
        memset(buff, 0, 256);
        strcat(buff, "TURN ");
        strcat(buff, fire_position);
        // buff[strlen(buff)] = '\0';
        printf("buff la %s\n", buff);
        int bytes_sent;
        bytes_sent = send(client_sock, buff, strlen(buff), 0);

        if (bytes_sent < 0)
            perror("\nError: ");

        // receive echo reply
        int bytes_received;
        while (1 == 1)
        {

            bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
            if (bytes_received < 0)
                perror("\nError: ");
            else if (bytes_received == 0)
                printf("Connection closed.\n");

            if (buff[0] == '0')
            {
                printf("Fired, please wait for enemy\n");
                continue;
            }
            buff[bytes_received] = '\0';

            printf("Message nhan duoc la %s\n", buff);
            break;
        }
    }
}