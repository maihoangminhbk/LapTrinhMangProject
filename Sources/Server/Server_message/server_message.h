#include <global_variable.h>

int handle_message(char*, char*);

int state_0_login(char*);

int state_1_createroom(char*, int, game_node*);

int state_1_joinroom(char*, int, game_node, int*);

int state_2_createship(char*, int, game_node, int*);

int state_3_fire(char*, char*, int, game_node*, int*);
