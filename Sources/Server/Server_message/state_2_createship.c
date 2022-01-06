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

int state_2_createship(char *buf, int fd, game_node game, int* recv_sock)
{
    // *game = AddTailGame(*game, fd);
    /*TODO:
        buf la data truyen vao, cam phai xu li
    */
    game_node game_node_get = GetGame(game, fd);
    if (fd == game_node_get->player1)
    {
        strcpy(game_node_get->data.ship_position_1, buf);
        *recv_sock = game_node_get->player2;

        if(game_node_get->data.ship_position_2[0] == '\0') {
            strcpy(buf, "3");
            buf[1] = '\0';
            return 1;
        }
    }

    if (fd == game_node_get->player2)
    {
        strcpy(game_node_get->data.ship_position_2, buf);
        *recv_sock = game_node_get->player1;

        if(game_node_get->data.ship_position_1[0] == '\0') {
            strcpy(buf, "3");
            buf[1] = '\0';
            return 1;
        }
    }

    strcpy(buf, "40");
    buf[2] = '\0';
    return 0;
}