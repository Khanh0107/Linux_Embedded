## Ghi nhận Trạng thái Process

Mô tả: Viết một chương trình tạo một tiến trình con, sau đó tiến trình cha sử dụng wait() để chờ tiến trình con kết thúc và ghi nhận trạng thái kết thúc của nó.

**Yêu cầu:**

- Trong tiến trình con, chọn trạng thái kết thúc bằng cách gọi exit() với các giá trị khác nhau.

- Sử dụng WIFEXITED() và WEXITSTATUS() để xác nhận và ghi nhận mã thoát của tiến trình con.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])   /* Cấp phát stack frame cho hàm main() */
{
    /* code */
    pid_t child_pid;                /* Lưu trong stack frame của main() */
    int status, ret;
    int retVal;
    ret = fork();         
    if (ret == 0)
    {
            /* Process con */
        printf("Im the child process, my PID: %d\n", getpid());
        printf("Child process terminate after 5 seconds\n");
	    sleep(5);
	    exit(43);
    } 
    else if(ret > 0)   
    {                  /* Process cha */
        retVal = wait(&status);
        printf("\nIm the parent process, PID child process: %d\n", retVal);   
        if (WIFEXITED(status))
        {
               printf("Normally termination, status = %d\n", WEXITSTATUS(status));
        }       
    } 
    else 
    {
        printf("fork() unsuccessfully\n");      // fork() return -1 nếu lỗi.
    }
    return 0;
}
```

## Input
```sh
gcc -o test main.c
./test
```
## Output
```sh
Im the child process, my PID: 4103
Child process terminate after 5 seconds

Im the parent process, PID child process: 4103
Normally termination, status = 43
```


