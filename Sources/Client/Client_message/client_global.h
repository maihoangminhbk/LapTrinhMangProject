#define ROW 10
#define COL 10

typedef struct client_map
{
    int home[ROW][COL];
    int away[ROW][COL];
    int count_ship;
    int turn;
} client_map;

