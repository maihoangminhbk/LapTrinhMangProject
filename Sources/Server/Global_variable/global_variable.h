#define ROW 10
#define COL 10

struct LinkedList{
    int connfd;
    char username[10];
    int state;
    struct LinkedList *next;
 };

typedef struct LinkedList *node; 
int GetByVal(node, int);
node DelByVal(node, int);
node AddTail(node, int, int, char*);
void Traverser(node);
int Search(node head, int fd);
node DelAt(node head, int position);
node Get(node head, int index);
typedef struct game_data {
    //char ship_position_1[10];
    //char ship_position_2[10];
    //char fire_1[10];
    //char fire_2[10];
    int home[ROW][COL];
    int away[ROW][COL];
    int count_ship;
    int turn;
    //char message[5];
} game_data;

struct GameList
{
    int player1;
    int player2;
    char room_name[20];
    int turn;
    game_data data1;
    game_data data2;
    struct GameList* next;
};

typedef struct GameList *game_node;


void setz(game_data*);
void get_ship_info(int[]);
int count_ships();
int count_ship_point();
game_data GetByValGame(game_node, int);
game_node DelByValGame(game_node, int);
game_node AddTailGame(game_node, int);
// game_data GetByRoomName(game_node, char*);
int SearchPlayerWithRoomName(game_node, int, char*);
game_node GetGame(game_node, int);
int SearchGameWithPlayer(game_node, int);
void TraverserGame(game_node);
game_node DelAtGame(game_node, int);


