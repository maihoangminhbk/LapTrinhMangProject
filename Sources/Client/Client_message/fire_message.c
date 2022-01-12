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
#include <client_global.h>

client_map map;
void setzz()
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            map.home[i][j] = 0;
            map.away[i][j] = 0;
        }
    }
    map.count_ship = 0;
    map.turn = 0;
}

void statuss(int value)
{
    if (value % 10 == 0)
    {
        printf(ANSI_COLOR_BLUE " ~~~ " ANSI_COLOR_RESET);
    }
    else if (value % 10 == 3)
    {
        printf(ANSI_COLOR_YELLOW "  X  " ANSI_COLOR_RESET);
    }
    else if (value % 10 == 4)
    {
        printf(ANSI_COLOR_RED "  O  " ANSI_COLOR_RESET);
    }
    else if (value % 10 == 1)
    {
        printf(" |#| ");
    }
    else
    {
        printf("error");
    }
}

void printMapp(int map[ROW][COL])
{
    int i, j;
    for (i = -1; i < ROW; i++)
    {
        for (j = -1; j < COL; j++)
        {
            if (i == -1 && j == -1)
            {
                printf(" x\\y ");
            }
            else if (i == -1)
            {
                printf(" %2d  ", j + 1);
            }
            else if (j == -1)
            {
                printf(" %2d  ", i + 1);
            }
            else
            {
                statuss(map[i][j]);
            }
        }
        printf("\n");
    }
}

int checkHitInput(int row, int col)
{
    if ((row <= 0) || (row > 10) || (col <= 0) || (col > 10))
    {
        return 0;
    }
    return 1;
}
// places left
int placescheckhome(int map[ROW][COL])
{
    int i, j, k = 0;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            if ((map[i][j] % 10) == 1)
            {
                k++;
            }
        }
    }
    return k;
}

// places left on the away
int placescheckaway()
{
    int i, j, k = 0, l = 0;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            if ((map.home[i][j] % 10) == 4 || (map.home[i][j] % 10) == 1)
            {
                k++;
            }
        }
    }

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            if ((map.away[i][j] % 10) == 4)
            {
                l++;
            }
        }
    }
    return (k - l);
}
// show screen when playing game after a turn
void gshow(char message[BUFF_SIZE])
{
	system("clear");
	printf("\n");
	printf("\nNETWORK BATTLESHIP V 1.0.1 Beta \n");
	printMapp(map.home);
	printf("Ships remaining to attack: %2d", placescheckaway());
	printf("\tShips left : %2d", placescheckhome(map.home));
	printf("\n");
	printMapp(map.away);
	puts(message);
}

int process_buff(char buff[])
{
    if (strcmp(buff, "4") == 0) {
        gshow("\nYou hit failed.\n");
    }
    else if (strcmp(buff, "5") == 0) {
        gshow("\nYou hit succeeded.\n");
    }
    else if (strcmp(buff, "6") == 0) {
        gshow("\nYou WON !! Awesome !! Keep it up :)\n");
        return 1;
    }
    else if (strcmp(buff, "7") == 0) {
        gshow("\nYou hit this position again.\n");
    }
    else if (strcmp(buff, "44") == 0) {
        gshow("\nThe opponent hit failed.\n");
    }
    else if (strcmp(buff, "55") == 0) {
        gshow("\nYou have been hit.\n");
    }
    else if (strcmp(buff, "66") == 0) {
        gshow("\nYou LOST. Better luck next time :( \n");
        return 2;
    }
    else if (strcmp(buff, "77") == 0) {
        gshow("\nThe opponent hit this position again.\n");
    }
    return 0;
}

int fire_message(int client_sock, int turn)
{
    char fire_position[20];
    char buff[256];
    int result = 0;
    memset(buff, 0, 256);
    setzz();
    int bytes_received;

    int x, y;
    if (turn == 0) {
        while (1==1)
        {
            printf("Please input fire position : \n");
            printf("Enter x = ");
            scanf("%d", &x);
            printf("Enter y = ");
            scanf("%d", &y);
            if (checkHitInput(x, y)) {
                int pos = x + y*10;
                
                memset(fire_position, 0, 20);
                sprintf(fire_position, "%d\n", pos);
                memset(buff, 0, 256);
                strcat(buff, "TURN ");
                strcat(buff, fire_position);
                buff[strlen(buff)-1] = '\0';

                int bytes_sent;
                bytes_sent = send(client_sock, buff, strlen(buff), 0);

                if (bytes_sent < 0)
                    perror("\nError: ");

                recv(client_sock, &map, sizeof(map), 0);

                memset(buff, 0, 256);
                bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
                buff[bytes_received] = '\0';
                
                result = process_buff(buff);
                if ( result == 1 || result == 2)
                {
                    return 0;
                }
                if (bytes_received < 0)
                    perror("\nError: ");
                else if (bytes_received == 0)
                    printf("Connection closed.\n");
                break;
            }
            else {
                printf("\nInvalid position.\n");
            }
        }
    }
    else {
            recv(client_sock, &map, sizeof(map), 0);
            memset(buff, 0, 256);
            int bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
            result = process_buff(buff);
            if ( result == 1 || result == 2)
            {
                return 0;
            }
            if (bytes_received < 0)
                perror("\nError: ");
            else if (bytes_received == 0)
                printf("Connection closed.\n");
    }

    while (result != 10)
    {
        if (map.turn == 0){
            while (1==1)
            {            
                printf("Please input fire position : \n");
                printf("Enter x = ");
                scanf("%d", &x);
                printf("Enter y = ");
                scanf("%d", &y);
                if (checkHitInput(x, y)) {
                    int pos = x + y*10;
                    memset(fire_position, 0, 20);
                    sprintf(fire_position, "%d\n", pos);
                    memset(buff, 0, 256);
                    strcat(buff, "TURN ");
                    strcat(buff, fire_position);
                    buff[strlen(buff)-1] = '\0';

                    int bytes_sent;
                    bytes_sent = send(client_sock, buff, strlen(buff), 0);

                    if (bytes_sent < 0)
                        perror("\nError: ");

                    // receive echo reply
                    int bytes_received;
                    recv(client_sock, &map, sizeof(map), 0);
                    memset(buff, 0, 256);
                    bytes_received = recv(client_sock, buff, BUFF_SIZE, 0); // 4 - 5 - 6
                    buff[bytes_received] = '\0';

                    int result = process_buff(buff);
                    if ( result == 1 || result == 2) // success or failed
                    {
                        result = 10;
                        return 0;
                    }
                    if (bytes_received < 0)
                        perror("\nError: ");
                    else if (bytes_received == 0)
                    printf("Connection closed.\n");
                    break;
                }
                else {
                    printf("\nInvalid position.\n");
                }
            }

        }
        else {
            recv(client_sock, &map, sizeof(map), 0);
            memset(buff, 0, 256);
            int bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
            int result = process_buff(buff);
            if (result == 1 || result == 2)
            {
                result = 10;
                return 0;
            }
            if (bytes_received < 0)
                perror("\nError: ");
            else if (bytes_received == 0)
            {
                printf("Connection closed.\n");
                exit(0);
            }
        }
    }
    return 0;
}