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
    int total_point = count_ship_point();
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
	return (total_point - k);
}
//update when attacking the enemy
int upstrike(int opp_home[ROW][COL], int my_away[ROW][COL], int x, int y)
{
	int a = opp_home[x][y] % 10;
	if (a == 1)
	{
		opp_home[x][y] = 4;
        my_away[x][y] = 4;
		if (placescheckaway(opp_home) == 0)
		{
			return 6;   // win
        }
		return 5;       // hit success
	}
	else if (a == 4 || a == 3)
	{
		return 7;       //hit this position again - failed
	}
	else
	{
		opp_home[x][y] = 3;
		my_away[x][y] = 3;
		return 4;       //hit failed
	}
}

int state_3_fire(char *buf, char *buf1, int fd, game_node *game, int* recv_sock)
{
    // *game = AddTailGame(*game, fd);
    /*TODO:
        buf la data truyen vao, cam phai xu li
    */
    int pos = 0;
    sscanf(buf, "%d", &pos);
    printf("pos = %d\n", pos);
    int x = pos % 10;
    printf("x = %d\n", x);
    int y = pos / 10;
    printf("y = %d\n", y);
    game_node game_node_get = GetGame(*game, fd);
    if (fd == game_node_get->player1)
    {
        //strcpy(game_node_get->data.fire_1, buf);
        int result = upstrike(game_node_get->data2.home, game_node_get->data1.away, x, y);
        if (result == 4) {
            strcpy(buf, "4"); // failed
            buf[1] = '\0';
            strcpy(buf1, "44");
            buf1[2] = '\0';
            game_node_get->data1.turn = 1;
            game_node_get->data2.turn = 0;
        }
        else if (result == 5) {
            strcpy(buf, "5"); // sucess
            buf[1] = '\0';
            strcpy(buf1, "55");
            buf1[2] = '\0';
            game_node_get->data1.turn = 0;
            game_node_get->data2.turn = 1;
        }
        else if (result == 6) {
            strcpy(buf, "6");
            buf[1] = '\0';
            strcpy(buf1, "66");
            buf1[2] = '\0';
            game_node_get->data1.turn = 0;
            game_node_get->data2.turn = 1;
        }
        else {
            strcpy(buf, "7"); // 
            buf[1] = '\0'; 
            strcpy(buf1, "77");
            buf1[2] = '\0';
            game_node_get->data1.turn = 1;
            game_node_get->data2.turn = 0;
        }
        *recv_sock = game_node_get->player2;
        //game_node_get->data1.turn = game_node_get->turn;
        //game_node_get->data2.turn = game_node_get->turn;
        write(fd, &game_node_get->data1, sizeof(game_node_get->data1));
        write(game_node_get->player2, &game_node_get->data2, sizeof(game_node_get->data2));
        //strcpy(buf, game_node_get->data.fire_1);
        if (result == 6) {
            int index = SearchGameWithPlayer(*game, fd);
            *game = DelAtGame(*game, index);
        }
        
    }

    if (fd == game_node_get->player2)
    {
        //strcpy(game_node_get->data.fire_2, buf);
        int result = upstrike(game_node_get->data1.home, game_node_get->data2.away, x, y);
        
        if (result == 4) {
            strcpy(buf, "4");
            buf[1] = '\0';
            strcpy(buf1, "44");
            buf1[2] = '\0';
            game_node_get->data1.turn = 0;
            game_node_get->data2.turn = 1;
        }
        else if (result == 5) {
            strcpy(buf, "5");
            buf[1] = '\0';
            strcpy(buf1, "55");
            buf1[2] = '\0';
            game_node_get->data1.turn = 1;
            game_node_get->data2.turn = 0;
        }
        else if (result == 6) {
            strcpy(buf, "6");
            buf[1] = '\0';
            strcpy(buf1, "66");
            buf1[2] = '\0';
            game_node_get->data1.turn = 1;
            game_node_get->data2.turn = 0;
        }
        else {
            strcpy(buf, "7");
            buf[1] = '\0';
            strcpy(buf1, "77");
            buf1[2] = '\0';
            game_node_get->data1.turn = 0;
            game_node_get->data2.turn = 1;
        }
        *recv_sock = game_node_get->player1;
        //game_node_get->data2.turn = game_node_get->turn;
        write(fd, &game_node_get->data2, sizeof(game_node_get->data2));
        write(game_node_get->player1, &game_node_get->data1, sizeof(game_node_get->data1));
        if (result == 6) {
            int index = SearchGameWithPlayer(*game, fd);
            *game = DelAtGame(*game, index);
        }

        //strcpy(buf, game_node_get->data.fire_1);
    }

    // strcpy(buf, game_node_get->data.fire_1);
    //buf[2] = '\0';
    return 0;
}