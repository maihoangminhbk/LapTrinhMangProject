#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#include <client_message.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BUFF_SIZE 1024

int main(){
	int client_sock;
	// char buff[BUFF_SIZE + 1];
	struct sockaddr_in server_addr; /* server's address information */
	// int msg_len, bytes_sent, bytes_received;
	
	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
		
	//Step 4: Communicate with server			
	
	//send message
	printf("\nWelcome to Battle Ship\n");
	printf("========================\n");
	
	
	if(!login_message(client_sock)) {
		printf("Login fails\n");
		return 0;
	}

	if(!option_message(client_sock)) {
		printf("Create room fails \n");
		return 0;
	}

	
	
	

	
}
