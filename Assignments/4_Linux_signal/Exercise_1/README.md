# Tín hiệu Process

**Mô tả :** 

Viết một chương trình tạo hai process cha - con. Tiến trình cha sẽ gửi tín hiệu SIGUSR1 tới tiến trình con sau một khoảng thời gian.

**Yêu cầu:**

Sử dụng signal() để định nghĩa hành vi của tiến trình con khi nhận tín hiệu SIGUSR1.

In ra một thông báo khi tiến trình con nhận được tín hiệu.

**Code**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sig_handler(int num)
{
    printf("Hello world \n");
    exit(2);
}

int main(int argc, char const *argv[])   /* Cấp phát stack frame cho hàm main() */
{
    /* code */
    pid_t child_pid;                /* Lưu trong stack frame của main() */
    child_pid = fork();         
    if (child_pid == 0)
    {
            /* Process con */
        printf("Im the child process, my PID: %d\n", getpid());
        signal(SIGUSR1, sig_handler);
        while(1);

    } 
    else if(child_pid > 0)   
    {                  /* Process cha */
        sleep(3);
        printf("\nIm the parent process, PID child process: %d\n", child_pid);  
        kill(child_pid, SIGUSR1);
        wait(NULL);
        
    } 
    else 
    {
        printf("fork() unsuccessfully\n");      // fork() return -1 nếu lỗi.
    }
    return 0;
}
```

**Output**

```sh
Im the child process, my PID: 4649

Im the parent process, PID child process: 4649
Hello world
```
