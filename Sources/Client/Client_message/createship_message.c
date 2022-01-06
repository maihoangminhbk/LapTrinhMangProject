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

int createship_message(int client_sock)
{
    int count_ship = 1;
    char ship_position[20];
    char buff[256];

    while (count_ship < 2)
    {
        printf("Please input ship position %d\n", count_ship);
        fgets(ship_position, 20, stdin);
        ship_position[strlen(ship_position) - 1] = '\0';
        memset(buff, 0, 256);
        strcat(buff, "CREATESHIP ");
        strcat(buff, ship_position);
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

            buff[bytes_received] = '\0';

            if (strcmp(buff, "41") == 0)
            {
                printf("Start game!!!\n");
                printf("Your turn\n");
                return 2;
            }

            if (strcmp(buff, "40") == 0)
            {
                printf("Start game!!!\n");
                printf("Wait...\n");
                return 1;
            }

            if (buff[0] == '3')
            {
                printf("Create ship successful\n");
                printf("Wait for other play create ship\n");
            }

            if (strcmp(buff, "30") == 0)
            {
                printf("Create ship fails\n");
                return 0;
            }
        }
    }
}