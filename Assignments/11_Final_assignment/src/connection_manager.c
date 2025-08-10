#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket. 
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>
#include "shared_data_pipe.h"
#include <pthread.h>

#define LISTEN_BACKLOG 50
#define BUFF_SIZE 256
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

extern int port_no;
struct data_sensor data_sen;

struct client_info {
    int sockfd;
    char ip[INET_ADDRSTRLEN];
    int port;
};

void *client_handler(void *arg) {
    struct client_info client = *(struct client_info *)arg;
    free(arg);

    struct data_sensor data;
    int n;

    while ((n = read(client.sockfd, &data, sizeof(data))) > 0) // client gui data.
    {
        // printf("[%s:%d] ID=%d, Time=%d, Temp=%d\n",
        //        client.ip, client.port,
        //        data.id, data.time, data.temperature);
        shr_data_pipe(&data);
    }
    if (n == 0) // client đóng kết nối.
    {
        printf("[%s:%d] Client disconnected.\n", client.ip, client.port);
    } else {
        perror("read");
    }

    close(client.sockfd);
    return NULL;
}

void *connect_tid(int *arg) 
{   
    int opt;
    int server_fd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket()");

    /* Ngăn lỗi : “address already in use” */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        handle_error("setsockopt()");  

    /* Khởi tạo địa chỉ cho server */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* Gắn socket với địa chỉ server */
    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind()");

    /* Nghe kết nối */
    if (listen(server_fd, LISTEN_BACKLOG) == -1)
        handle_error("listen()");

    printf("Server is listening at port: %d\n", port_no);

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        struct client_info *info = malloc(sizeof(struct client_info));
        info->sockfd = client_fd;
        inet_ntop(AF_INET, &client_addr.sin_addr, info->ip, sizeof(info->ip));
        info->port = ntohs(client_addr.sin_port);

        printf("New client connected: %s:%d\n", info->ip, info->port);

        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, info);
        pthread_detach(tid);
    }

    close(server_fd);
    return NULL;
}

