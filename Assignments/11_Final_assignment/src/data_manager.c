#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shared_data_pipe.h"
#include "data_manager.h"
#include <pthread.h>

#define THERSHHOLD_HOT 30
#define THERSHHOLD_COLD 22

extern int fd[2];
extern int fd_log[2];
struct data_sensor data_pipe_read;
struct data_sensor data_buff[10] ;
char flag_proc = 0;
float temp_aver = 0;
int temp_sum = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition = 0; // 0 = xu li , 1 = storage

void *process_tid(int *arg) 
{
    while (1)
    {
        ssize_t n = read(fd[0], &data_pipe_read, sizeof(data_pipe_read));
        if (n == -1) {
            perror("read from pipe failed");
            continue;
        } else if (n == 0) {
            printf("Pipe closed by writer.\n");
            break;
        }
        if (flag_proc <= 10)
        {
            pthread_mutex_lock(&mutex);
            
            while (condition == 1) 
            {
            pthread_cond_wait(&cond, &mutex);
            }
        // Lưu nhiệt độ vào buffer
            data_buff[flag_proc].id = data_pipe_read.id;
            data_buff[flag_proc].time = data_pipe_read.time;
            data_buff[flag_proc].temperature = data_pipe_read.temperature;
            
            // printf("\nSensor ID read pipe: ID= %d, time =%d, temperature: %d", 
            //     data_pipe_read.id, data_pipe_read.time, data_pipe_read.temperature);
            printf("\nSensor ID read pipe: ID= %d, time =%d, temperature: %d", 
                data_buff[flag_proc].id, data_buff[flag_proc].time, data_buff[flag_proc].temperature);
            // printf("\nflag_proc = %d", flag_proc);
            flag_proc++;
            if (flag_proc == 10)
            {
                
                temp_sum = 0;
                for (int i = 0; i < 10; i++) 
                {
                    temp_sum += data_buff[i].temperature;
                }
                temp_aver = temp_sum / 10.0;

                printf("\nAverage temperature = %.2f", temp_aver);

                if (temp_aver > THERSHHOLD_HOT) 
                {
                    printf(" => HOT!\n");
                } else if (temp_aver < THERSHHOLD_COLD) 
                {
                    printf(" => COLD!\n");
                } else {
                    printf(" => NORMAL.\n");
                }

                flag_proc = 0;  // reset counter
                condition = 1;
                pthread_cond_signal(&cond);
                // printf("im here");
            }
            pthread_mutex_unlock(&mutex);   
        }   
    }
}

