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

// #define ROW 10
// #define COL 10

// typedef struct client_map
// {
//     int home[ROW][COL];
//     int away[ROW][COL];
// } client_map;
// client_map mymap;
client_map mymap;
void setz()
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            mymap.home[i][j] = 0;
            mymap.away[i][j] = 0;
        }
        
    }
}

int checkShipInput(int row, int col, int ori)
{
	if ((row <= 0) || (row > 10) || (col <= 0) || (col > 10))
	{
		return 0;
	}
	if ((ori != 0) && (ori != 1))
	{
		return 0;
	}
	return 1;
}

// char graphics later upgrade to open gl
void status(int value)
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

void printMap(int map[ROW][COL])
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
				status(map[i][j]);
			}
		}
		printf("\n");
	}
}

int createship_message(int client_sock)
{
    int count_ship = 1;
    char ship_position[20];
    char buff[256];

    int a, b, c;
    while (count_ship <= 2)
    {
        printf("Enter x coordinate for placement of ship, size = %2d: ", count_ship);
		scanf("%d", &a);
		printf("Enter y coordinate for placement of ship, size = %2d: ", count_ship);
		scanf("%d", &b);
		printf("Enter the orientation for placement of ship \n0 = horizontal , 1 = vertical, size = %2d: ", count_ship);
		scanf("%d", &c);
        //printf("Please input ship position %d\n", count_ship);
        //fgets(ship_position, 20, stdin);
        //ship_position[strlen(ship_position) - 1] = '\0';
        if (checkShipInput(a,b,c)){
            int pos = a + b*10 + count_ship*100 + c*1000;
            //char ship_position[10];
            memset(ship_position, 0, 20);
            sprintf(ship_position, "%d\n", pos);
            memset(buff, 0, 256);
            strcat(buff, "CREATESHIP ");
            strcat(buff, ship_position);
            count_ship++;
            buff[strlen(buff)-1] = '\0';
            printf("buff la %s\n", buff);
            int bytes_sent;
            bytes_sent = send(client_sock, buff, strlen(buff), 0);

            if (bytes_sent < 0)
                perror("\nError: ");

        // receive echo reply
            int bytes_received;
            setz();
            //while (1 == 1)
            //{
                recv(client_sock, &mymap.home, sizeof(mymap.home), 0);
                printMap(mymap.home);
                
                bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
                
               // char dt[10];
                //recv(client_sock, buff)

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
                    //printf("Wait for other play create ship\n");
                }

                if (strcmp(buff, "30") == 0)
                {
                    printf("Create ship fails\n");
                    //return 0;
                }
            //}
        }
    }
    return 0;
}