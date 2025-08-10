#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shared_data_pipe.h"

struct data_sensor data_pipe;
extern int fd[2];
extern int fd_log[2];
void shr_data_pipe(struct data_sensor *data)
{  
    
    

    data_pipe = *data;
    // printf("\nSensor ID= %d, time =%d, temperature: %d" ,data_pipe.id, data_pipe.time, data_pipe.temperature);
    // close(fd[0]); ko can
    write(fd[1], &data_pipe, sizeof(data_pipe));
    write(fd_log[1], &data_pipe, sizeof(data_pipe));
    // close(fd[1])


}
