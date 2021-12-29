#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <server_message.h>

int handle_message(char* buf, char* data) {
    char* data_solve;
    char * func = strtok(buf, " ");
    memset(data, 0, 100);
    data_solve = strtok(NULL, " ");
    strcpy(data, data_solve);
    // printf("Func la %s", func);
    if(strcmp(func, "LOGIN") == 0) {
        return 0;
    }

    if(strcmp(func, "CREATEROOM") == 0) {
        return 1;
    }

    if(strcmp(func, "JOINROOM") == 0) {
        return 2;
    }

    if(strcmp(func, "EXIT") == 0) {
        return 3;
    }

    return 0;
}