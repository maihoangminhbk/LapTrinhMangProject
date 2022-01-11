#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <global_variable.h>
int ship_info[] = {1};

int count_ships()
{
    int i = 0;
	int count = 0;
	while(ship_info[i] >= 1){
		count++;
		i++;
	}
	return count;
}

void get_ship_info(int info[])
{
    int total = count_ships();
    int i = 0;
    for (i = 0; i < total; i++)
    {
        info[i] = ship_info[i];
    }
}
//
int count_ship_point()
{
    int ship_number = count_ships();
	int i, sum = 0;
	for(i = 0; i < ship_number; i++)
	{
		sum += ship_info[i];
	}
    return sum;
}

void setz(game_data *map)
{
	int i = 0, j = 0;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			(*map).home[i][j] = 0;
            (*map).away[i][j] = 0;
		}
	}
	//memset(&map.message, 0, MAXLINE);
}

game_node CreateNodeGame(int fd)
{
    game_node temp;                                      // declare a node
    temp = (game_node)malloc(sizeof(struct GameList)); // Cấp phát vùng nhớ dùng malloc()
    temp->next = NULL;                                   // Cho next trỏ tới NULL
    temp->player1 = fd;                             // Gán giá trị cho Node
    temp->player2 = 0;
    temp->turn = 0;
    char roomname[20];
    memset(roomname, 0, 20);
    char id[6];
    memset(id, 0, 6);
    //memset(temp->data.ship_position_1, 0, 10);
    //memset(temp->data.ship_position_2, 0, 10);
    setz(&temp->data1);
    setz(&temp->data2);
    temp->data1.count_ship = 0;
    temp->data2.count_ship = 0;
    // itoa(fd, id, 10);
    sprintf(id, "%d", fd);
    strcat(roomname, "room");
    strcat(roomname, id);
    strcpy(temp->room_name, roomname);

    return temp; // Trả về node mới đã có giá trị
}

game_node AddTailGame(game_node head, int fd)
{
    game_node temp, p;            // Khai báo 2 node tạm temp và p
    temp = CreateNodeGame(fd); // Gọi hàm createNode để khởi tạo node temp có next trỏ tới NULL và giá trị là value
    if (head == NULL)
    {
        head = temp; // Nếu linked list đang trống thì Node temp là head luôn
    }
    else
    {
        p = head; // Khởi tạo p trỏ tới head
        while (p->next != NULL)
        {
            p = p->next; // Duyệt danh sách liên kết đến cuối. Node cuối là node có next = NULL
        }
        p->next = temp; // Gán next của thằng cuối = temp. Khi đó temp sẽ là thằng cuối(temp->next = NULL mà)
    }
    return head;
}

game_node DelHeadGame(game_node head)
{
    if (head == NULL)
    {
        printf("\nCha co gi de xoa het!");
    }
    else
    {
        head = head->next;
    }
    return head;
}

game_node DelTailGame(game_node head)
{
    if (head == NULL || head->next == NULL)
    {
        return DelHeadGame(head);
    }
    game_node p = head;
    while (p->next->next != NULL)
    {
        p = p->next;
    }
    p->next = p->next->next; // Cho next bằng NULL
    // Hoặc viết p->next = NULL cũng được
    return head;
}

game_node DelAtGame(game_node head, int position)
{
    if (position == 0 || head == NULL || head->next == NULL)
    {
        head = DelHeadGame(head); // Nếu vị trí chèn là 0, tức là thêm vào đầu
    }
    else
    {
        // Bắt đầu tìm vị trí cần chèn. Ta sẽ dùng k để đếm cho vị trí
        int k = 1;
        game_node p = head;
        while (p->next->next != NULL && k != position)
        {
            p = p->next;
            ++k;
        }

        if (k != position)
        {
            // Nếu duyệt hết danh sách lk rồi mà vẫn chưa đến vị trí cần chèn, ta sẽ mặc định xóa cuối
            // Nếu bạn không muốn xóa, hãy thông báo vị trí xóa không hợp lệ
            head = DelTailGame(head);
            // printf("Vi tri xoa vuot qua vi tri cuoi cung!\n");
        }
        else
        {
            p->next = p->next->next;
        }
    }
    return head;
}

game_node GetGame(game_node head, int index)
{
    int k = 0;
    game_node p = head;
    while (p->next != NULL && k != index)
    {
        ++k;
        p = p->next;
    }
    return p;
}

int SearchGameWithPlayer(game_node head, int fd)
{
    int position = 0;
    for (game_node p = head; p != NULL; p = p->next)
    {
        if (p->player1 == fd || p->player2 == fd)
        {
            return position;
        }
        ++position;
    }
    return -1;
}

int SearchPlayerWithRoomName(game_node head, int fd, char* roomname)
{
    //int position = 0;
    // printf("Check join room search\n");
    for (game_node p = head; p != NULL; p = p->next)
    {
        printf("room name -%s- - -%s-\n", p->room_name, roomname);
        if (strcmp(p->room_name, roomname) == 0)
        {
            printf("Player 1 la %d\n", p->player1);
            p->player2 = fd;
            return p->player1;
        } else {
            printf("Khong bang nhau\n");
        }
        
    }
    return -1;
}

game_node DelByValGame(game_node head, int fd)
{
    int position = SearchGameWithPlayer(head, fd);
    while (position != -1)
    {
        DelAtGame(head, position);
        position = SearchGameWithPlayer(head, fd);
    }
    return head;
}

game_data GetByValGame(game_node head, int fd)
{
    int position = SearchGameWithPlayer(head, fd);

    game_data data = GetGame(head, position)->data1;
    return data;
}

// game_data GetByRoomName(game_node head, char* roomname)
// {
//     int position = SearchGameWithRoomName(head, roomname);

//     game_data data = GetGame(head, position);
//     return data;
// }

game_node InitHeadGame()
{
    game_node head;
    head = NULL;
    return head;
}

void TraverserGame(game_node head)
{
    printf("\n");
    for (game_node p = head; p != NULL; p = p->next)
    {
        printf("player 1 id is %5d \troom name is %s\n", p->player1, p->room_name);
    }
}