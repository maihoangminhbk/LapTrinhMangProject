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
#include <client_global.h>
#define BUFF_SIZE 1024

client_map mymap;

// setting the arrays to zero in the begining
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
// check that the input is correct value or not
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
		printf(" ~X~ ");        // hit failed
	}
	else if (value % 10 == 4)
	{
		printf(" /O/ ");        // hit successful
	}
	else if (value % 10 == 1)
	{
		printf(" /#/ ");        // ship
	}
	else
	{
		printf("error");
	}
}
// print map to screen
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
// count number of ships
int count_ship(int ship_info[])
{
    int i = 0;
	int count = 0;
	while(ship_info[i] >= 1){
		count++;
		i++;
	}
	return count;
}
//
int createship_message(int client_sock)
{
    int index = 0;
    char ship_position[20];
    char buff[256];
    int ship_info[256];

    recv(client_sock, &ship_info, BUFF_SIZE, 0);
    int ship_numbers = count_ship(ship_info);

    int row, col, ori, size;    // row, column, orientation, size of ship
    while (index <= ship_numbers)
    {
        size = ship_info[index];
        printf("Enter x coordinate for placement of ship, size = %2d: ", size);
		scanf("%d", &row);
		printf("Enter y coordinate for placement of ship, size = %2d: ", size);
		scanf("%d", &col);
		printf("Enter the orientation for placement of ship \n0 = horizontal , 1 = vertical, size = %2d: ", size);
		scanf("%d", &ori);

        if (checkShipInput(row, col, ori)){
            int pos = row + col*10 + size*100 + ori*1000;
            memset(ship_position, 0, 20);
            sprintf(ship_position, "%d\n", pos);
            memset(buff, 0, 256);
            strcat(buff, "CREATESHIP ");
            strcat(buff, ship_position);
            buff[strlen(buff)-1] = '\0';
            printf("buff la %s\n", buff);
            // send position of ship to server
            int bytes_sent;
            bytes_sent = send(client_sock, buff, strlen(buff), 0);

            if (bytes_sent < 0)
                perror("\nError: ");

            // receive reply
            int bytes_received;
            setz();
            recv(client_sock, &mymap.home, sizeof(mymap.home), 0);  // receive map
            printMap(mymap.home);
                
            bytes_received = recv(client_sock, buff, BUFF_SIZE, 0); // receive message, success or fail

            if (bytes_received < 0)
                perror("\nError: ");
            else if (bytes_received == 0)
                printf("Connection closed.\n");

            buff[bytes_received] = '\0';

            if (strcmp(buff, "41") == 0)
            {
                printf("Start game!!!\n");
                printf("Your turn\n");
                return 0;
            }

            if (strcmp(buff, "40") == 0)
            {
                printf("Start game!!!\n");
                printf("You are the guest, so play next turn\n");
                printf("Wait...\n");
                return 1;
            }

            if (strcmp(buff, "42") == 0)
            {
                printf("Waiting for the player 2...\n");
                return 0;
            }
                
            if (strcmp(buff, "3") == 0)
            {
                printf("Created ship successfully\n");
                index++;
            }

            if (strcmp(buff, "30") == 0)
            {
                printf("Created ship failed\n");        
            }
        }
        else {
            printf("\nInvalid position.\n");
        }
    }
    return 0;
}