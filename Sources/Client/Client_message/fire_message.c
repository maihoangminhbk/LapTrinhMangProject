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
		printf(" ~~~ ");
	}
	else if (value % 10 == 3)
	{
		printf(" ~X~ ");
	}
	else if (value % 10 == 4)
	{
		printf(" /O/ ");
	}
	else if (value % 10 == 1)
	{
		printf(" /#/ ");
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

int checkHitInput(int row, int col){
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
int placescheckaway(int map[ROW][COL])
{
	int i, j, k = 0;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if ((map[i][j] % 10) == 4)
			{
				k++;
			}
		}
	}
	return (3 - k);
}

void gshow(char message[BUFF_SIZE])
{
	//system("clear");
	//int i = -1, j = -1, k, l;
	printf("\n");
	printf("\nNETWORK BATTLESHIP V 1.0.1 Beta \n");
	printMapp(map.home);
	printf("Ships remaining to attack: %2d", placescheckaway(map.away));
	printf("\tShips left : %2d", placescheckhome(map.home));
	printf("\n");
	printMapp(map.away);
	//sleep(2);
	puts(message);
	//sleep(2);
}

int process_buff(char buff[])
{
    if (strcmp(buff, "4") == 0) {
        gshow("You hit failed.\n");
    }
    else if (strcmp(buff, "5") == 0) {
        gshow("You hit succeeded.\n");
    }
    else if (strcmp(buff, "6") == 0) {
        gshow("You WON !! Awesome !! Keep it up :)\n");
        return 1;
    }
    else if (strcmp(buff, "7") == 0) {
        gshow("You hit this position again.\n");
    }
    else if (strcmp(buff, "44") == 0) {
        gshow("The opponent hit failed.\n");
    }
    else if (strcmp(buff, "55") == 0) {
        gshow("You have been hit.\n");
    }
    else if (strcmp(buff, "66") == 0) {
        gshow("You LOST. Better luck next time :( \n");
        return 2;
    }
    else if (strcmp(buff, "77") == 0) {
        gshow("The opponent hit this position again.\n");
    }
    return 0;
}

int fire_message(int client_sock, int turn)
{
    char fire_position[20];
    char buff[256];
    char buff1[10];
    memset(buff1, 0, 10);
    int result = 0;
    //memset(buff, 0, 256);
    setzz();
    int bytes_received;
    //setz(&map);
    int x, y;
    if (turn == 0) {
        bytes_received = recv(client_sock, buff, BUFF_SIZE, 0); ///buff = 41
        printf("Please input fire position : ");
        printf("Enter x = ");
        scanf("%d", &x);
        printf("Enter y = ");
        scanf("%d", &y);
        //fgets(fire_position, 20, stdin);
        //fire_position[strlen(fire_position) - 1] = '\0';

        int pos = x + y*10;
        //char fire_position[10];
        memset(fire_position, 0, 20);
        sprintf(fire_position, "%d\n", pos);
        memset(buff, 0, 256);
        strcat(buff, "TURN ");
        strcat(buff, fire_position);
        buff[strlen(buff)-1] = '\0';
        printf("buff la %s\n", buff);
        int bytes_sent;
        bytes_sent = send(client_sock, buff, strlen(buff), 0);

        if (bytes_sent < 0)
            perror("\nError: ");

        // receive echo reply
        
        //while (1 == 1)
        //{
            recv(client_sock, &map, sizeof(map), 0);
            //gshow("");
            //send(client_sock, "TURN _", strlen("TURN _"), 0);
            memset(buff, 0, 256);
            bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
            printf("byte = %d\n", bytes_received);
            printf("buff1 = %s\n", buff);
            buff[bytes_received] = '\0';
            printf("buff2 = %s\n", buff);
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
    else {
            //printf("haha\n");
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

    while ( result != 10 )
    {
        if (map.turn == 0){
            printf("1Please input fire position : ");
            printf("Enter x = ");
            scanf("%d", &x);
            printf("Enter y = ");
            scanf("%d", &y);
            //fgets(fire_position, 20, stdin);
            //fire_position[strlen(fire_position) - 1] = '\0';

            int pos = x + y*10;
            //char fire_position[10];
            memset(fire_position, 0, 20);
            sprintf(fire_position, "%d\n", pos);
            memset(buff, 0, 256);
            strcat(buff, "TURN ");
            strcat(buff, fire_position);
            buff[strlen(buff)-1] = '\0';
            printf("buff la %s\n", buff);
            int bytes_sent;
            bytes_sent = send(client_sock, buff, strlen(buff), 0);

            if (bytes_sent < 0)
                perror("\nError: ");

            // receive echo reply
            int bytes_received;
            //while (1 == 1)
            //{
            recv(client_sock, &map, sizeof(map), 0);
            //gshow("");
            //send(client_sock, "TURN _", strlen("TURN _"), 0);
            memset(buff, 0, 256);
            bytes_received = recv(client_sock, buff, BUFF_SIZE, 0); // 4 - 5 - 6
            printf("byte = %d\n", bytes_received);
            printf("buff1 = %s\n", buff);
            buff[bytes_received] = '\0';
            printf("buff2 = %s\n", buff);
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

        }
        else {
            printf("hihi\n");
            recv(client_sock, &map, sizeof(map), 0);
            memset(buff, 0, 256);
            int bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
            int result = process_buff(buff);
            if ( result == 1 || result == 2)
            {
                result = 10;
                return 0; 
            }
            if (bytes_received < 0)
                perror("\nError: ");
            else if (bytes_received == 0)
                printf("Connection closed.\n");
        }
    }
    return 0;
}