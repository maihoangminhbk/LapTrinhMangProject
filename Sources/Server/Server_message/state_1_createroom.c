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

int state_1_createroom(char *buf, int fd, game_node *game)
{
    // *game = AddTailGame(*game, fd);
    if (buf[0] == '_')
    {
        *game = AddTailGame(*game, fd);
    }

    if (strcmp(buf, "cancel") == 0)
    {
        int index = SearchGameWithPlayer(*game, fd);
        *game = DelAtGame(*game, index);
    }

    strcpy(buf, "5");
    buf[1] = '\0';
    return 1;
}