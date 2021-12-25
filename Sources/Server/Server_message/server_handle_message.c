#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <server_handle_message.h>

int handle_message(char* buf, char* data) {
    char* data_solve;
    char * func = strtok(buf, " ");
    memset(data, 0, 100);
    data_solve = strtok(NULL, " ");
    strcpy(data, data_solve);
    
    if(strcmp(func, "LOGIN") == 0) {
        return 0;
    }
}