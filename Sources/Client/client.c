#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#include <client_global.h>
#include <client_message.h>

//#define SERVER_ADDR "127.0.0.1"
//#define SERVER_PORT 5550
#define BUFF_SIZE 1024

int main(int argc, char **argv)
{
	int client_sock;

	int server_port = atoi(argv[1]);
	char server_address[20];
	memset(server_address, 0, 20);
	strcpy(server_address, argv[2]);
	// char buff[BUFF_SIZE + 1];
	struct sockaddr_in server_addr; /* server's address information */
	// int msg_len, bytes_sent, bytes_received;

	// Step 1: Construct socket
	client_sock = socket(AF_INET, SOCK_STREAM, 0);

	
	// Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(server_address);

	// Step 3: Request to connect server
	if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
	{
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}

	// Step 4: Communicate with server

	// send message
	printf("\n============================================================================================\n");
	printf("Welcome to Battle Ship\n");
	printf("Battle Ship (also Battleships or Sea Battle) is a strategy type guessing game for two players.\nafter adjusting the setting you can choose your shots to destroy rival's ships and gain score\nHave fun!\n\n");
    printf (BLUE "XXXXX   XXXX  XXXXXX XXXXXX XX     XXXXXX  XXXXX XX  XX XX XXXX\n" RESET);
	printf (BLUE "XX  XX XX  XX   XX     XX   XX     XX     XX     XX  XX XX XX  XX\n" RESET);
	printf (BLUE "XXXXX  XX  XX   XX     XX   XX     XXXX    XXXX  XXXXXX XX XXXX\n" RESET); 
	printf (BLUE "XX  XX XXXXXX   XX     XX   XX     XX         XX XX  XX XX XX\n" RESET);
	printf (BLUE "XXXXX  XX  XX   XX     XX   XXXXXX XXXXXX XXXXX  XX  XX XX XX\n\n" RESET);
	printf("==============================================================================================\n");
	int op = 0;
	int turn = 0;
	while (op != 4)
	{
		switch (op)
		{
		case 0:
			if (!login_message(client_sock))
			{
				printf("\nLogin failed\n");
				return 0;
			}
			op = 1;
			break;
		case 1:
			op = option_message(client_sock);
			if (op == 0)
			{
				op = 1;
				break;
			}
			if (op == 2)
			{
				op = 0;
				break;
			}

			if (op == 1)
			{
				op = 2;
				break;
			}

		case 2:
			turn = createship_message(client_sock);
			op = 3;
			break;

		case 3:
			fire_message(client_sock, turn);
			op = 1;
			break;
		default:
			break;
		}
	}
}
