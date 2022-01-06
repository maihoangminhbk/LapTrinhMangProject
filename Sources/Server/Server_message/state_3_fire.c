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

int state_3_fire(char *buf, int fd, game_node game, int* recv_sock)
{
    // *game = AddTailGame(*game, fd);
    /*TODO:
        buf la data truyen vao, cam phai xu li
    */
    game_node game_node_get = GetGame(game, fd);
    if (fd == game_node_get->player1)
    {
        strcpy(game_node_get->data.fire_1, buf);
        *recv_sock = game_node_get->player2;
        strcpy(buf, game_node_get->data.fire_1);

    }

    if (fd == game_node_get->player2)
    {
        strcpy(game_node_get->data.fire_2, buf);
        *recv_sock = game_node_get->player1;
        strcpy(buf, game_node_get->data.fire_1);


    }

    // strcpy(buf, game_node_get->data.fire_1);
    buf[2] = '\0';
    return 0;
}