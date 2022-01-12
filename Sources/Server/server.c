#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <poll.h>
#include <limits.h>

#include <server_message.h>
// #include <global_variable.h>

#define LISTENQ 5
#define BUF_SIZE 1024

node head = NULL;
game_node game_head = NULL;

// write "n" bytes to a descriptor

ssize_t writen(int fd, char *ptr, size_t n)
{
        ssize_t nleft = n, nwritten;

        while (nleft > 0)
        {
                if ((nwritten = write(fd, ptr, nleft)) <= 0)
                {
                        if (nwritten < 0 && errno == EINTR)
                        {
                                nwritten = 0; /* and call write() again */
                        }
                        else
                        {
                                return -1;
                        }
                }

                nleft -= nwritten;
                ptr += nwritten;
        }

        return n;
}

void usage(char *program)
{
        fprintf(stderr, "usage: %s port\n", program);
}

int main(int argc, char **argv)
{
        int nready, i, maxi, port, listenfd, connfd, sockfd;
        socklen_t clilen;
        struct sockaddr_in cliaddr, servaddr;
        char buf[BUF_SIZE];
        // const int OPEN_MAX = sysconf(_SC_OPEN_MAX);  // maximum number of opened files
        struct pollfd clients[OPEN_MAX];
        ssize_t n;
        int INFTIM = -1;

        if (argc != 2)
        {
                usage(argv[0]);
                return EXIT_FAILURE;
        }

        // Get TCP port number
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535)
        {
                fprintf(stderr, "Invalid port number %d\n", port);
                return EXIT_FAILURE;
        }

        // Create listen socket
        if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                fprintf(stderr, "Error: socket\n");
                return EXIT_FAILURE;
        }
        else
        {
                printf("Create listen socket %d\n", listenfd);
        }

        // Initialize server socket address
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(port);

        // Bind socket to an address
        if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
                fprintf(stderr, "Error: bind\n");
                return EXIT_FAILURE;
        }

        // Listen
        if (listen(listenfd, LISTENQ) < 0)
        {
                fprintf(stderr, "Error: listen\n");
                return EXIT_FAILURE;
        }

        clients[0].fd = listenfd;
        clients[0].events = POLLRDNORM;

        for (i = 1; i < OPEN_MAX; i++)
        {
                clients[i].fd = -1; // -1 indicates available entry
        }
        maxi = 0; // max index into clients[] array

        while (1)
        {
                nready = poll(clients, maxi + 1, INFTIM);

                if (nready <= 0)
                {
                        continue;
                }

                // Check new connection
                if (clients[0].revents & POLLRDNORM)
                {
                        clilen = sizeof(cliaddr);
                        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
                        {
                                fprintf(stderr, "Error: accept\n");
                                return EXIT_FAILURE;
                        }

                        printf("Accept socket %d (%s : %hu)\n",
                               connfd,
                               inet_ntoa(cliaddr.sin_addr),
                               ntohs(cliaddr.sin_port));

                        // Save client socket into clients array
                        for (i = 0; i < OPEN_MAX; i++)
                        {
                                if (clients[i].fd < 0)
                                {
                                        clients[i].fd = connfd;
                                        break;
                                }
                        }

                        // No enough space in clients array
                        if (i == OPEN_MAX)
                        {
                                fprintf(stderr, "Error: too many clients\n");
                                close(connfd);
                        }

                        clients[i].events = POLLRDNORM;

                        if (i > maxi)
                        {
                                maxi = i;
                        }

                        // No more readable file descriptors
                        if (--nready <= 0)
                        {
                                continue;
                        }
                }

                // Check all clients to read data
                for (i = 1; i <= maxi; i++)
                {
                        if ((sockfd = clients[i].fd) < 0)
                        {
                                continue;
                        }

                        // If the client is readable or errors occur
                        if (clients[i].revents & (POLLRDNORM | POLLERR))
                        {
                                n = read(sockfd, buf, BUF_SIZE);
                                buf[n] = '\0';

                                if (n < 0)
                                {
                                        fprintf(stderr, "Error: read from socket %d\n", sockfd);
                                        close(sockfd);
                                        clients[i].fd = -1;
                                }
                                else if (n == 0)
                                { // connection closed by client
                                        printf("Close socket %d\n", sockfd);
                                        close(sockfd);
                                        int index = SearchGameWithPlayer(game_head, sockfd);
                                        game_head = DelAtGame(game_head, index);
                                        // head = DelByVal(head, sockfd);

                                        clients[i].fd = -1;
                                }
                                else
                                {
                                        printf("Read %zu bytes from socket %d\n", n, sockfd);
                                        
                                        int ship_info[10];
                                        get_ship_info(ship_info);
                                        char data[100];
                                        memset(data, 0, 100);
                                        char data_opp[100];
                                        memset(data_opp, 0, 100);
                                        printf("Check\n");
                                        int message_func = handle_message(buf, data);
                                        printf("data -%s-\n", data);
                                        // printf("Check 2\n");
                                        printf("Buff la %s, func no la %d\n", buf, message_func);
                                        // if(message_func != 2) {
                                        //         printf("T bang 2 roi day -%d-\n", message_func);
                                        // }
                                        int recv_sock;
                                        int result;
                                        switch (message_func)
                                        {
                                        case 0:
                                                if (state_0_login(data))
                                                {
                                                        head = AddTail(head, sockfd, 0);
                                                        // head = AddTail(head, sockfd, 0);
                                                        Traverser(head);
                                                };
                                                n = strlen(data);
                                                printf("data la %s\n", data);
                                                printf("sockfd = %d\n", sockfd);
                                                write(sockfd, data, n);

                                                break;
                                        case 1:
                                                // game_head = AddTailGame(game_head, sockfd);
                                                // game_head = AddTailGame(game_head, sockfd);
                                                state_1_createroom(data, sockfd, &game_head);
                                                // TraverserGame(game_head);
                                                TraverserGame(game_head);

                                                n = strlen(data);
                                                printf("data la %s\n", data);
                                                printf("data la %s\n", data);
                                                printf("sockfd = %d\n", sockfd);
                                                write(sockfd, data, n);
                                                break;
                                        case 2:
                                                // int index = SearchPlayerWithRoomName(game_head, data);
                                                
                                                //int index1 = 0;
                                                //printf("index la %d\n", index1);
                                                
                                                if(state_1_joinroom(data, sockfd, game_head, &recv_sock)) {
                                                        write(recv_sock, "30", 2);
                                                }

                                                n = strlen(data);
                                                printf("data la %s\n", data);
                                                printf("sockfd = %d\n", sockfd);
                                                write(sockfd, data, n);
                                                break;
                                        case 4:
                                                if (strcmp(data, "_") == 0) 
                                                {
                                                        write(sockfd, &ship_info, sizeof(ship_info));
                                                }
                                                else {
                                                result = state_2_createship(data, sockfd, game_head, &recv_sock);
                                                printf("result = %d\n", result);
                                                n = strlen(data);
                                                printf("data la %s\n", data);
                                                printf("sockfd = %d\n", sockfd);
                                                write(sockfd, data, n);
                                                }
                                                break;

                                        case 5:
                                                state_3_fire(data, data_opp, sockfd, &game_head, &recv_sock);
                                                write(recv_sock, data_opp, strlen(data_opp));
                                                // data[0] = '0';
                                                n = strlen(data);
                                                printf("data la %s b\n", data);
                                                printf("sockfd = %d\n", sockfd);
                                                write(sockfd, data, n);
                                                break;

                                        default:
                                                break;
                                        }
                                }

                                // No more readable file descriptors
                                if (--nready <= 0)
                                {
                                        break;
                                }
                        }
                }
        }

        close(listenfd);
        return EXIT_SUCCESS;
}