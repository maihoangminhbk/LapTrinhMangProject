

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

typedef struct game_data {
    char message[10];
} game_data;

struct GameList
{
    int player1;
    int player2;
    char room_name[20];
    game_data data;
    struct GameList* next;
};

typedef struct GameList *game_node;
game_data GetByValGame(game_node, int);
game_node DelByValGame(game_node, int);
game_node AddTailGame(game_node, int);
// game_data GetByRoomName(game_node, char*);
int SearchPlayerWithRoomName(game_node, char*);
game_data GetGame(game_node, int);
void TraverserGame(game_node);

