#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <global_variable.h>


 
node CreateNode(int fd, int state){
    node temp; // declare a node
    temp = (node)malloc(sizeof(struct LinkedList)); // Cấp phát vùng nhớ dùng malloc()
    temp->next = NULL;// Cho next trỏ tới NULL
    temp->connfd = fd; // Gán giá trị cho Node
    temp->state = state;
    return temp;//Trả về node mới đã có giá trị
}

node AddTail(node head, int fd, int state){
    node temp,p;// Khai báo 2 node tạm temp và p
    temp = CreateNode(fd, state);//Gọi hàm createNode để khởi tạo node temp có next trỏ tới NULL và giá trị là value
    if(head == NULL){
        head = temp;     //Nếu linked list đang trống thì Node temp là head luôn
    }
    else{
        p  = head;// Khởi tạo p trỏ tới head
        while(p->next != NULL){
            p = p->next;//Duyệt danh sách liên kết đến cuối. Node cuối là node có next = NULL
        }
        p->next = temp;//Gán next của thằng cuối = temp. Khi đó temp sẽ là thằng cuối(temp->next = NULL mà)
    }
    return head;
}

node DelHead(node head){
    if(head == NULL){
        printf("\nCha co gi de xoa het!");
    }else{
        head = head->next;
    }
    return head;
}

node DelTail(node head){
    if (head == NULL || head->next == NULL){
         return DelHead(head);
    }
    node p = head;
    while(p->next->next != NULL){
        p = p->next;
    }
    p->next = p->next->next; // Cho next bằng NULL
    // Hoặc viết p->next = NULL cũng được
    return head;
}

node DelAt(node head, int position){
    if(position == 0 || head == NULL || head->next == NULL){
        head = DelHead(head); // Nếu vị trí chèn là 0, tức là thêm vào đầu
    }else{
        // Bắt đầu tìm vị trí cần chèn. Ta sẽ dùng k để đếm cho vị trí
        int k = 1;
        node p = head;
        while(p->next->next != NULL && k != position){
            p = p->next;
            ++k;
        }
 
        if(k != position){
            // Nếu duyệt hết danh sách lk rồi mà vẫn chưa đến vị trí cần chèn, ta sẽ mặc định xóa cuối
            // Nếu bạn không muốn xóa, hãy thông báo vị trí xóa không hợp lệ
            head = DelTail(head);
            // printf("Vi tri xoa vuot qua vi tri cuoi cung!\n");
        }else{
            p->next = p->next->next;
        }
    }
    return head;
}

int Get(node head, int index){
    int k = 0;
    node p = head;
    while(p->next != NULL && k != index){
        ++k;
        p = p->next;
    }
    return p->state;
}

int Search(node head, int fd){
    int position = 0;
    for(node p = head; p != NULL; p = p->next){
        if(p->connfd == fd){
            return position;
        }
        ++position;
    }
    return -1;
}

node DelByVal(node head, int fd){
    int position = Search(head, fd);
    while(position != -1){
        DelAt(head, position);
        position = Search(head, fd);
    }
    return head;
}

int GetByVal(node head, int fd){
    int position = Search(head, fd);
    
    int state = Get(head, position);
    return state;
}

node InitHead(){
    node head;
    head = NULL;
    return head;
}

void Traverser(node head){
    printf("\n");
    for(node p = head; p != NULL; p = p->next){
        printf("fd is %5d \tstate is %5d\n", p->connfd, p->state);
    }
}

