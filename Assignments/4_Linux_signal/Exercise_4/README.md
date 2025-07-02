# Tạo Giao tiếp giữa Các Process bằng Tín hiệu

**Yêu cầu:** 

Viết chương trình tạo hai process (cha và con) và sử dụng tín hiệu SIGUSR1 để giao tiếp giữa chúng.

**Cách thực hiện**

Dùng fork() để tạo process con từ process cha.

Process cha sẽ gửi tín hiệu SIGUSR1 cho process con mỗi 2 giây.

Khi nhận tín hiệu SIGUSR1, process con sẽ in ra thông báo "Received signal from parent".

Dừng chương trình sau khi đã gửi tín hiệu 5 lần.

**Gợi ý:**

Dùng kill() để gửi tín hiệu từ process cha đến process con, và sử dụng signal() trong process con để bắt tín hiệu SIGUSR1.

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int counter = 0;
void handle_SIGUSR1(int sig) 
{
    counter++;

    if (counter < 5) {
        printf("Received signal from parent \n");
        printf("Counter: %d \n", counter);
    } 
    else if (counter == 5)
    {
        counter =0;
        printf(" Kết thúc chương trình.\n");
        exit(0);
    }
}

int main() 
{
    pid_t pid;  
    pid = fork();
    if (pid == 0) 
    {
           /* Process con */
        printf("My PID is: %d\n", getpid());
        signal(SIGUSR1, handle_SIGUSR1);
        while(1);
        
    } 
    else if (pid > 0)
    {                    /* Process cha */
        printf("\nIm the parent process\n");
        printf("My PID is: %d\n", getpid());

        sleep(1);

        for (int i = 0; i < 5; i++) {
            sleep(2);  // Chờ 2 giây
            kill(pid, SIGUSR1);  // Gửi tín hiệu đến con
        }

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
Im the parent process
My PID is: 3880
My PID is: 3881
Received signal from parent 
Counter: 1 
Received signal from parent 
Counter: 2 
Received signal from parent 
Counter: 3 
Received signal from parent 
Counter: 4 
 Kết thúc chương trình.
```
