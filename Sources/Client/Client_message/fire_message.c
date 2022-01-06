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

// client_map mymap;
// void setz()
// {
//     int i, j;
//     for (i = 0; i < ROW; i++)
//     {
//         for (j = 0; j < COL; j++)
//         {
//             mymap.home[i][j] = 0;
//             mymap.away[i][j] = 0;
//         }
        
//     }
// }

// void status(int value)
// {
// 	if (value % 10 == 0)
// 	{
// 		printf(" ~~~ ");
// 	}
// 	else if (value % 10 == 3)
// 	{
// 		printf(" ~X~ ");
// 	}
// 	else if (value % 10 == 4)
// 	{
// 		printf(" /O/ ");
// 	}
// 	else if (value % 10 == 1)
// 	{
// 		printf(" /#/ ");
// 	}
// 	else
// 	{
// 		printf("error");
// 	}
// }

// void printMap(int map[ROW][COL])
// {
//     int i, j;
// 	for (i = -1; i < ROW; i++)
// 	{
// 		for (j = -1; j < COL; j++)
// 		{
// 			if (i == -1 && j == -1)
// 			{
// 				printf(" x\\y ");
// 			}
// 			else if (i == -1)
// 			{
// 				printf(" %2d  ", j + 1);
// 			}
// 			else if (j == -1)
// 			{
// 				printf(" %2d  ", i + 1);
// 			}
// 			else
// 			{
// 				status(map[i][j]);
// 			}
// 		}
// 		printf("\n");
// 	}
// }

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
	printMap(mymap.home);
	printf("Ships remaining to attack: %2d", placescheckaway(mymap.away));
	printf("\tShips left : %2d", placescheckhome(mymap.home));
	printf("\n");
	printMap(mymap.away);
	//sleep(2);
	puts(message);
	//sleep(2);
}

int fire_message(int client_sock, int turn)
{
    char fire_position[20];
    char buff[256];

    setz();
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
    
    int x, y;
    while (1 == 1)
    {
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
        buff[strlen(buff)] = '\0';
        printf("buff la %s\n", buff);
        int bytes_sent;
        bytes_sent = send(client_sock, buff, strlen(buff), 0);

        if (bytes_sent < 0)
            perror("\nError: ");

        // receive echo reply
        int bytes_received;
        //while (1 == 1)
        //{
            recv(client_sock, &mymap, sizeof(mymap), 0);
            bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
            if (bytes_received < 0)
                perror("\nError: ");
            else if (bytes_received == 0)
                printf("Connection closed.\n");

            if (strcpy(buff, "4")) {
                gshow("You hit failed.\n");
            }
            else if (strcpy(buff, "5")) {
                gshow("You hit succeeded.\n");
            }
            else if (strcpy(buff, "6")) {
                gshow("You WON !! Awesome !! Keep it up :)\n");
            }
            else if (strcpy(buff, "7")) {
                gshow("You hit this position again.\n");
            }
            else if (strcpy(buff, "44")) {
                gshow("The opponent hit failed.\n");
            }
            else if (strcpy(buff, "55")) {
                gshow("You have been hit.\n");
            }
            else if (strcpy(buff, "66")) {
                gshow("You LOST. Better luck next time :( \n");
            }
            else if (strcpy(buff, "77")) {
                gshow("The opponent hit this position again.\n");
            }
            
            if (buff[0] == '0')
            {
                printf("Fired, please wait for enemy\n");
                continue;
            }
            buff[bytes_received] = '\0';

            printf("Message nhan duoc la %s\n", buff);
            //break;
        //}
    }
    //return 0;
}