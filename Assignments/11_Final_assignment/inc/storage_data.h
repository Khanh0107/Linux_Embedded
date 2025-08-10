
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shared_data_pipe.h"
#include "data_manager.h"
#include <pthread.h>

extern struct data_sensor data_buff[10] ;
extern int condition;
extern pthread_mutex_t mutex ;
extern pthread_cond_t cond;


void *storage_data();
