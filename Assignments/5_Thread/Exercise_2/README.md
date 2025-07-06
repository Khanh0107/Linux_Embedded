**Question:**

Viết một chương trình thực hiện tạo 3 threads. 

Thread thứ nhất thực hiện việc nhập dữ liệu sinh viên từ bàn phím, bao gồm thông tin: Họ tên, ngày sinh, quê quán.

 Mỗi lần nhập xong dữ liệu một sinh viên, thread thứ hai sẽ ghi thông tin sinh viên đó vào file (mỗi thông tin sinh viên nằm trên 1 dòng) thongtinsinhvien.txt. 
 
Thread thứ 3 sẽ đọc dữ liệu vừa ghi được và in ra terminal rồi thông báo cho thread 1 tiếp tục nhập thêm sinh viên.

Sử dụng mutex và condition variable để giải quyết bài toán.

```c
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h> 

struct student {
    char name [20];
    int birthyear;
    char hometown[20];
} student1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int shared_data = 0;

void *thread_function_1(void *arg) 
{   
    pthread_mutex_lock(&mutex);
    
    printf("Input the name of student: \n");
    scanf("%s", student1.name);
    printf("Input the birth year of student: \n");
    scanf("%d", &student1.birthyear);
    printf("Input the hometown of student: \n");
    scanf("%s", student1.hometown);
    shared_data = 1; // Update shared data
    
    pthread_cond_signal(&cond); // 
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_function_2(void *arg) 
{
    int fd = open("thongtinhocsinh.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    pthread_mutex_lock(&mutex);
    while (shared_data != 1)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    char buffer[128];
    snprintf(buffer, sizeof(buffer),
             "Name student is %s\nBirth year is %d\nHome town is %s\n",
             student1.name, student1.birthyear, student1.hometown);
    
    write(fd,buffer,strlen(buffer));
    shared_data = 2;
    pthread_cond_signal(&cond); // 
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_function_3(void *arg) 
{
    usleep(1000); 
    pthread_mutex_lock(&mutex);
    while (shared_data != 2)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Name student is %s\nBirth year is %d\nHome town is %s\n",
             student1.name, student1.birthyear, student1.hometown);
    shared_data = 0;
    pthread_mutex_unlock(&mutex);
    return NULL;
}


int main() {

    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, thread_function_1, NULL);
    pthread_create(&thread2, NULL, thread_function_2, NULL);
    pthread_create(&thread3, NULL, thread_function_3, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    return 0;
}
```
