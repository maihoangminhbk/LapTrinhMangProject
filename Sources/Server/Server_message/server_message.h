#include <global_variable.h>

typedef struct user
{
    char userid[20];
    char password[200];
    int status;
    struct user *next;
} User;

User *searchUser(User *users, char *userid);


int handle_message(char*, char*);

int state_0_login(char*, User**, char*);

int state_1_createroom(char*, int, game_node*);

int state_1_joinroom(char*, int, game_node, int*);

int state_2_createship(char*, int, game_node, int*);

int state_3_fire(char*, char*, int, game_node*, int*);
