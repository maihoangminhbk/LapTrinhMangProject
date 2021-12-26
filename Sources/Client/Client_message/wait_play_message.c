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

int wait_play_message(int client_sock) {
    printf("Please wait for other play\n");
    
}