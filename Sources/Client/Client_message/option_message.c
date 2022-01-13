#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>

#include <client_message.h>
#define BUFF_SIZE 1024

int count_down(float time)
{
    clock_t begin;
    double time_spent;
    unsigned int i;

    /* Mark beginning time */
    begin = clock();
    for (i = 0; 1; i++)
    {
        // printf("hello\n");
        /* Get CPU time since loop started */
        time_spent = (double)(clock() - begin) / CLOCKS_PER_SEC;
        if (time_spent >= time)
            break;
    }
    return 0;
}

int option_message(int client_sock)
{
    char buff[256];
    char roomname[20];

    int input = 0;
    while (input == 0)
    {
        printf("==============================================================================================\n");
        printf("1. Create Room\n");
        printf("2. Join room\n");
        printf("3. Exit\n");
        printf("Input option to play:\n");

        memset(buff, 0, 256);

        scanf("%d%*c", &input);

        switch (input)
        {
        case 1:
            printf("\nChoose create room\n");
            strcat(buff, "CREATEROOM _");
            break;

        case 2:
            printf("Input room name:\n");
            memset(roomname, 0, 20);
            fgets(roomname, 20, stdin);
            roomname[strlen(roomname) - 1] = '\0';
            strcat(buff, "JOINROOM ");
            strcat(buff, roomname); // JOINROOM room4
            break;

        case 3:
            printf("Log out...\n");
            strcat(buff, "EXIT _");
            break;

        default:
            printf("Input from 1 ~ 3\n");
            input = 0;
            break;
        }
    }

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

    int on;
    switch (buff[0])
    {
    case '5':
        on = 1;
        int rc = ioctl(client_sock, FIONBIO, (char *)&on);
        if (rc < 0)
        {
            perror("ioctl() failed");
            close(client_sock);
            exit(0);
        }

        printf("Create room successful\n");

        int time = 0;
        do
        {
            printf("Wait for other player\n");
            bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
            if (bytes_received < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    perror("  recv() failed");
                    close(client_sock);
                    exit(0);
                }
                count_down(3.0);
                printf("Wait in 3s...\n");
                time += 3;
                if (time % 30 == 0)
                {
                    printf("No one joins the game\n");
                    memset(buff, 0, 256);
                    strcat(buff, "CREATEROOM cancel");

                    on = 0;
                    rc = ioctl(client_sock, FIONBIO, (char *)&on);
                    if (rc < 0)
                    {
                        perror("ioctl() failed");
                        close(client_sock);
                        exit(0);
                    }
                    int bytes_sent;
                    bytes_sent = send(client_sock, buff, strlen(buff), 0);
                    // printf("Buff gui di la %s\n", buff);
                    if (bytes_sent < 0)
                        perror("\nError: ");
                    bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
                    // printf("bytes receive la %d\n", bytes_received);
                    // printf("Buff tra ve la %s\n", buff);
                    

                    return 0;
                }

                continue;
            }

            else if (bytes_received == 0)
                printf("Connection closed.\n");

            on = 0;
            rc = ioctl(client_sock, FIONBIO, (char *)&on);
            if (rc < 0)
            {
                perror("ioctl() failed");
                close(client_sock);
                exit(0);
            }

            buff[bytes_received] = '\0';
            if (strcmp(buff, "30") == 0)
            {
                printf("Find game successful... Let's goooooo\n");
                return 1;
            }
        } while (1 == 1);
        printf("check break\n");
        break;

    case '6':
        printf("Join room successful\n");
        return 1;
        break;
    case '1':
        printf("Not found room name, Join room fails\n");
        return 0;
        break;

    default:
        break;
    }
    return 0;
}