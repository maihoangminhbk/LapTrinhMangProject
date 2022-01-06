#define ROW 10
#define COL 10

typedef struct client_map
{
    int home[ROW][COL];
    int away[ROW][COL];
} client_map;
// client_map mymap;

int login_message(int);

int option_message(int);

int createship_message(int);

//int fire_message(int, int);
