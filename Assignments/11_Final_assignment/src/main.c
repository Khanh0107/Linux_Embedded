#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket. 
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "connection_manager.h"
#include "data_manager.h"
#include "storage_data.h"
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h> 

int port_no;
int fd[2];
int fd_log[2];

int main(int argc, char *argv[]) 
{
    pid_t pid;
    // fd[0] để đọc, fd[1] để ghi
    if (pipe(fd) == -1 || pipe(fd_log) == -1) 
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
     
    // Tạo process con
    pid = fork();

    if (pid < 0) {
        // fork thất bại
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Tiến trình con: log process
        struct data_sensor buff;
        char buffer[50]; // bộ đệm đủ lớn để chứa chuỗi
        printf("Child process: My PID is %d\n", getpid());
        int fd_txt = open("log.txt", O_RDWR | O_CREAT | O_APPEND, 0666); // Open or create file
        if (fd_txt < 0) 
        {
            printf("Error opening file!\n");
            return 1;
        }
        while (1)
        {
            int n = read(fd_log[0], &buff, sizeof(buff));
            // printf("Read %d bytes: ID=%d, time=%d, temperature=%d\n",
            //     n, buff.id, buff.time, buff.temperature);

            snprintf(buffer, sizeof(buffer), "sensor id = %d: timestamp = %d temperature = %d\n", buff.id, buff.time, buff.temperature);
            write(fd_txt, buffer, strlen(buffer));
        }
    } else {
        // Tiến trình cha
        /* Đọc portnumber trên command line */
        if (argc < 2) 
        {
            printf("No port provided\ncommand: ./server <port number>\n");
            exit(EXIT_FAILURE);
        }       
        port_no = atoi(argv[1]); // convert string to integer

        printf("Parent process: Child PID is %d\n", pid);
        pthread_t thread1, thread2, thread3;
        pthread_create(&thread1, NULL, connect_tid, NULL);
        pthread_create(&thread2, NULL, process_tid, NULL);
        pthread_create(&thread3, NULL, storage_data, NULL);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
    }
    return 0;
}
