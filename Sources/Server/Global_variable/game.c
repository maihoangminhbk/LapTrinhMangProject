#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <global_variable.h>

game_node CreateNodeGame(int fd)
{
    game_node temp;                                      // declare a node
    temp = (game_node)malloc(sizeof(struct LinkedList)); // Cấp phát vùng nhớ dùng malloc()
    temp->next = NULL;                                   // Cho next trỏ tới NULL
    temp->player1 = fd;                             // Gán giá trị cho Node
    temp->player2 = 0;
    char roomname[20];
    memset(roomname, 0, 20);
    char id[6];
    memset(id, 0, 6);
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

game_data GetGame(game_node head, int index)
{
    int k = 0;
    game_node p = head;
    while (p->next != NULL && k != index)
    {
        ++k;
        p = p->next;
    }
    return p->data;
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

    game_data data = GetGame(head, position);
    return data;
}

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