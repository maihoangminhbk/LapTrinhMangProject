#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define ROW 10
#define COL 10

typedef struct client_map
{
    int home[ROW][COL];
    int away[ROW][COL];
    int count_ship;
    int turn;
} client_map;

