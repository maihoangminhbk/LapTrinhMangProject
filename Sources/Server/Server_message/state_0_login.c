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

typedef struct user
{
    char userid[10];
    char password[10];
    int status;
    struct user *next;
} User;
User *users;

User *initUser(char *userid, char *password, int status)
{
    User *newUser = (User *)malloc(sizeof(User));
    if (newUser == NULL)
    {
        printf("Allocated error!\n");
        return NULL;
    }
    strcpy(newUser->userid, userid);
    strcpy(newUser->password, password);
    newUser->status = status;
    newUser->next = NULL;
    return newUser;
}
void addUser(User **users, User *user)
{
    User *cur = *users;
    if (*users == NULL)
    {
        *users = user;
        return;
    }
    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = user;
    return;
}
User *searchUser(User *users, char *userid)
{
    User *cur = users;
    while (cur != NULL)
    {
        if (strcmp(cur->userid, userid) == 0)
        {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

// process read file
void readUserFromFile(User **users, char *filename)
{
    char userid[20];
    char password[20];
    int status;
    char line[50];
    FILE *fo = fopen(filename, "r");
    if (fo == NULL)
    {
        printf("Open file error\n");
        return;
    }
    while (fgets(line, 50, fo) != NULL)
    {
        char *token = strtok(line, "|");
        strcpy(userid, token);
        token = strtok(NULL, "|");
        strcpy(password, token);
        token = strtok(NULL, "|");
        status = atoi(token);
        User *user = initUser(userid, password, status);
        bzero(userid, 20);
        bzero(password, 20);
        addUser(users, user);
    };
    fclose(fo);
    return;
}
void writeToFile(User *users, char *filename)
{
    FILE *fo = fopen(filename, "w");
    if (fo == NULL)
    {
        printf("File open failed\n");
        return;
    }
    User *cur = users;
    while (cur != NULL)
    {
        printf("%s %s %d\n", cur->userid, cur->password, cur->status);

        fprintf(fo, "%s|%s|%d\n", cur->userid, cur->password, cur->status);
        cur = cur->next;
    }
    fclose(fo);
    return;
}
void printUser(User *users)
{
    User *cur = users;
    while (cur != NULL)
    {
        printf("%s %s %d\n", cur->userid, cur->password, cur->status);
        cur = cur->next;
    }
}
int authenticate(User *users, char *userid, char *password)
{
    User *user = searchUser(users, userid);
    if (user == NULL)
    {
        printf("User not found\n");
        return 0;
    }
    if (strcmp(user->password, password) == 0)
    {
        return 1;
    }
    return 0;
}

int state_0_login(char *buf)
{

    char userid[20];
    char password[20];

    bzero(userid, 20);
    bzero(password, 20);
    users = NULL;
	readUserFromFile(&users, "Server_message/accounts.txt");


    char *token = strtok(buf, "|");
    strcpy(userid, token);

    token = strtok(NULL, "|");
    strcpy(password, token);

    User *user = searchUser(users, userid);

    
    if (user == NULL)
    {
        strcpy(buf, "0 0");
        return 0;

    }

    if (user->status == 0)
    {
        strcpy(buf, "0 2");
        return 0;
    }

    
    int check = authenticate(users, userid, password);

    
    if (check)
    {

        strcpy(buf, "1");
        user->status = 0;
        writeToFile(users, "test.txt");
        return 1;
    }
    else
    {
        strcpy(buf, "0 1");
        return 0;
    }
}