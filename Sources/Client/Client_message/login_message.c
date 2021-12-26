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

int login_message(int client_sock)
{
    char userid[20];
    char password[20];
    char response[256];
    char buff[256];
    memset(response, 0, 256);
    // receive data from the server
    // int bytes = 0;
    int count = 0;
    while (count < 5)
    {
        memset(buff, 0, 256);

        printf("Enter user id:\n");
        fgets(userid, 20, stdin);
        printf("Enter pssword:\n");
        fgets(password, 20, stdin);
        userid[strlen(userid) - 1] = '\0';
        password[strlen(password) - 1] = '\0';

        strcat(buff, "LOGIN ");
        strcat(buff, userid);
        strcat(buff, "|");
        strcat(buff, password);
        
        int bytes_sent;
        bytes_sent = send(client_sock, buff, strlen(buff), 0);
        
        if (bytes_sent < 0)
            perror("\nError: ");

        // receive echo reply
        int bytes_received;
        bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
        if (bytes_received < 0)
            perror("\nError: ");
        else if (bytes_received == 0)
            printf("Connection closed.\n");

        buff[bytes_received] = '\0';
        if (buff[0] == '0')
        {
            switch (buff[2])
            {
            case '0':
                printf("Invalid username\n");
                break;
            case '1':
                printf("Invalid password\n");
                break;
            case '2':
                printf("Other login\n");
                break;

            default:
                break;
            }
        }
        else
        {
            printf("login successful\n");
            // return 1;
        }


        bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
        if (bytes_received < 0)
            perror("\nError: ");
        else if (bytes_received == 0)
            printf("Connection closed.\n");

        buff[bytes_received] = '\0';
        if (buff[0] == '0')
        {
            switch (buff[2])
            {
            case '0':
                printf("Invalid username\n");
                break;
            case '1':
                printf("Invalid password\n");
                break;
            case '2':
                printf("Other login\n");
                break;

            default:
                break;
            }
        }
        else
        {
            printf("login successful\n");
            return 1;
        }
    }
    return 0;
}
