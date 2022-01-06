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
	else
	{
		return 8;
	}
}

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


int processShip(int map[ROW][COL], int row, int col, int ori, int size)
{
	if ((checkfill(map, (row - 1), (col - 1), ori, size) == 1))
	{
		addship(map, (row - 1), (col - 1), ori, size);
		//post(1);
		//sendMap();
		return 1;
	}
	else
	{
		//post(0);
		//sendMap();
		return 0;
	}
}

int state_2_createship(char *buf, int fd, game_node game, int* recv_sock)
{
    // *game = AddTailGame(*game, fd);
    /*TODO:
        buf la data truyen vao, cam phai xu li
    */
    int pos = 0;
    sscanf(buf, "%d", &pos);
    int a, b, c, d;
    if (pos >= 1000)
	{
		a = (pos % 1000) % 100 % 10;
		printf("a = %d\n", a);
		b = ((pos % 1000) % 100) / 10;
		printf("b = %d\n", b);
        d = (pos % 1000) / 100;
        printf("d = %d\n", d);
		c = 1;
		// if (processShip(,a,b,c,d))
		// {
	    // 	d++;
		// }
	}
	else
	{
	    a = ( pos % 100) % 10;
        printf("a = %d\n", a);
		b = ( pos % 100) / 10;
        printf("b = %d\n", b);
        d = pos / 100;
        printf("d = %d\n", d);
		c = 0;
		// if (processShip(a,b,c,d))
		// {
	    // 	d++;
		// }
	}
    game_node game_node_get = GetGame(game, fd);
    if (fd == game_node_get->player1)
    {
        //strcpy(game_node_get->data.ship_position_1, buf);
        *recv_sock = game_node_get->player2;
        int result = processShip(game_node_get->data1.home, a, b, c, d);
        //char data[5];
        //sprintf(data, "%d\n", result);
        if (result == 1) {
            strcpy(buf, "3");
            buf[1] = '\0';
            //return 1;
            game_node_get->data1.count_ship++;
        }
        else {
            strcpy(buf, "30");
            buf[2] = '\0';
        }
        //write(fd, buf, strlen(buf));

        write(fd, &game_node_get->data1.home, sizeof(game_node_get->data1.home));
        // if(game_node_get->data.ship_position_2[0] == '\0') {
        //     strcpy(buf, "3");
        //     buf[1] = '\0';
        //     return 1;
        // }
        if (game_node_get->data1.count_ship == 2){
            if (game_node_get->data2.count_ship == 2)
            {
                strcpy(buf, "41");
                buf[2] = '\0';
                return 0;
            }
            strcpy(buf, "40");
            buf[2] = '\0';
        }
        //return 1;
    }

    if (fd == game_node_get->player2)
    {
        //strcpy(game_node_get->data.ship_position_2, buf);
        *recv_sock = game_node_get->player1;

        int result = processShip(game_node_get->data2.home, a, b, c, d);
        //char data[5];
        //sprintf(data, "%d\n", result);
        if (result == 1) {
            strcpy(buf, "3");
            buf[1] = '\0';
            //return 1;
            game_node_get->data2.count_ship++;
            
        }
        else {
            strcpy(buf, "30");
            buf[2] = '\0';
        }
        //write(fd, buf, strlen(buf));

        write(fd, &game_node_get->data2.home, sizeof(game_node_get->data2.home));

        // if(game_node_get->data.ship_position_1[0] == '\0') {
        //     strcpy(buf, "3");
        //     buf[1] = '\0';
        //     return 1;
        // }
        if (game_node_get->data2.count_ship == 2)
        {
            if (game_node_get->data1.count_ship == 2)
            {
                strcpy(buf, "41");
                buf[2] = '\0';
                return 0;
            }
            strcpy(buf, "40");
            buf[2] = '\0';
        }
        //return 1;
    }
    //strcpy(buf, "40");
    //buf[2] = '\0';
    return 1;
}