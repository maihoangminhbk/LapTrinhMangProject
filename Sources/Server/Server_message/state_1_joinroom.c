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
// #include <global_variable.h>


int state_1_joinroom(char *buf, int fd, game_node game, int* recv_sock) {
    
    int index = SearchPlayerWithRoomName(game, fd, buf);
    // TraverserGame(game);
    
    
    printf("index la %d\n", index);
    
    if(index == -1) {
        strcpy(buf, "1");
        return 0;
    } else {
        *recv_sock = index;
    }

    strcpy(buf, "6");
    buf[1] = '\0';
    return 1;

}