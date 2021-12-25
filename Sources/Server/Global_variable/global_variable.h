#include <stdio.h>

struct LinkedList{
    int connfd;
    int state;
    struct LinkedList *next;
 };

typedef struct LinkedList *node; //Từ giờ dùng kiểu dữ liệu LinkedList có thể thay bằng node cho ngắn gọn

int GetByVal(node, int);
node DelByVal(node, int);
node AddTail(node head, int fd, int state);
void Traverser(node);

