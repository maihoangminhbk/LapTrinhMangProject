#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <poll.h>
#include <limits.h>

#include <server_message.h>

//checking if the ship can be inserted in the given position
int checkfill(int map[ROW][COL], int x, int y, int orientation, int size)
{
	int i, k = 0;
	if (orientation == 1)
	{
		for (i = 0; ((x + i) < 10) && (i < size); i++)
		{
			if (map[x + i][y] == 0)
			{
				k++;
			}
		}
	}
	else if (orientation == 0)
	{
		for (i = 0; ((y + i) < 10) && (i < size); i++)
		{
			if (map[x][y + i] == 0)
			{
				k++;
			}
		}
	}
	if (k == size)
	{
		return 1;
	}
    return 0;
}
// insert ship
int addship(int map[ROW][COL], int x, int y, int orientation, int size)
{
	int i = 0;
	if (orientation == 1)
	{
		for (i = 0; i < size; i++)
		{
			map[x + i][y] = 10 * (x + i) + 100 * (y) + 1000 * (size) + 1;
		}
	}
	else
	{
		for (i = 0; i < size; i++)
		{
			map[x][y + i] = 10 * (x) + 100 * (y + i) + 1000 * (size) + 1;
		}
	}
    return 0;
}
// process to create ship
int processShip(int map[ROW][COL], int row, int col, int ori, int size)
{
	if ((checkfill(map, (row - 1), (col - 1), ori, size) == 1))
	{
		addship(map, (row - 1), (col - 1), ori, size);
		return 1;
	}
	else
	{
		return 0;
	}
}
//
int state_2_createship(char *buf, int fd, game_node game, int* recv_sock)
{
    // *game = AddTailGame(*game, fd);
    /*TODO:
        buf la data truyen vao, cam phai xu li
    */
    int pos = 0;
    sscanf(buf, "%d", &pos); // convert data from string to int
    int row, col, ori, size; // row, column, orientation, size of ship
    int total_ship = count_ships(); 
    printf("total ship = %d\n", total_ship);
    if (pos >= 1000)
	{
		row = (pos % 1000) % 100 % 10;
		printf("a = %d\n", row);
		col = ((pos % 1000) % 100) / 10;
		printf("b = %d\n", col);
        size = (pos % 1000) / 100;
        printf("d = %d\n", size);
		ori = 1;
	}
	else
	{
	    row = ( pos % 100) % 10;
        printf("a = %d\n", row);
		col = ( pos % 100) / 10;
        printf("b = %d\n", col);
        size = pos / 100;
        printf("d = %d\n", size);
		ori = 0;
	}
    game_node game_node_get = GetGame(game, fd);
    // if fd is socket of player 1
    if (fd == game_node_get->player1)
    {
        *recv_sock = game_node_get->player2;
        int result = processShip(game_node_get->data1.home, row, col, ori, size);
        if (result == 1) {
            strcpy(buf, "3"); // created ship successfully
            buf[1] = '\0';
            game_node_get->data1.count_ship++;
        }
        else {
            strcpy(buf, "30"); // created ship failed
            buf[2] = '\0';
        }
        // send home map to player 1
        write(fd, &game_node_get->data1.home, sizeof(game_node_get->data1.home));
        // if player 1 created ships fully
        if (game_node_get->data1.count_ship == total_ship){
            if (game_node_get->data2.count_ship == total_ship)
            {
                strcpy(buf, "41");
                buf[2] = '\0';
                return 1;
            }
            strcpy(buf, "42");
            buf[2] = '\0';
        }
    }
    // if fd is socket of player 2
    if (fd == game_node_get->player2)
    {
        *recv_sock = game_node_get->player1;
        int result = processShip(game_node_get->data2.home, row, col, ori, size);
        if (result == 1) {
            strcpy(buf, "3");   // created ship successfully
            buf[1] = '\0';
            game_node_get->data2.count_ship++;
        }
        else {
            strcpy(buf, "30");  // created ship successfully
            buf[2] = '\0';
        }
        // send home map to player 2
        write(fd, &game_node_get->data2.home, sizeof(game_node_get->data2.home));
        // if player 2 created ships fully
        if (game_node_get->data2.count_ship == total_ship)
        {
            if (game_node_get->data1.count_ship == total_ship)
            {
                strcpy(buf, "40");
                buf[2] = '\0';
                return 0;
            }
            strcpy(buf, "40");
            buf[2] = '\0';
        }
    }
    return 2;
}