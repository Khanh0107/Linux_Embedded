# Sử dụng Pipe để giao tiếp giữa Parent and Children Process

**Yêu cầu:** 

Viết một chương trình sử dụng pipe để giao tiếp giữa hai tiến trình.

Tạo một pipe và fork một tiến trình con.

Tiến trình cha sẽ gửi một chuỗi văn bản vào pipe.

Tiến trình con sẽ đọc chuỗi văn bản từ pipe và in ra màn hình.

**Gợi ý:**

Sử dụng hàm pipe() để tạo pipe, fork() để tạo tiến trình con, và read()/write() để giao tiếp giữa các tiến trình.

**Code**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MSG_SIZE    100

char * message = "From parent: HELLO child";

char pipe_buff[MSG_SIZE];
int fds[2];

int main(int argc, char const *argv[])   /* Cấp phát stack frame cho hàm main() */
{
    pid_t child_pid;                /* Lưu trong stack frame của main() */

    if (pipe(fds) < 0) {
        printf("pipe() unsuccessfully\n");
        exit(1);
    }

    child_pid = fork();         
    if (child_pid >= 0) {
        if (0 == child_pid) 
        {       /* Process con */
            close(fds[1]);
            read(fds[0], pipe_buff, MSG_SIZE);
            printf("%s\n", pipe_buff);
            close(fds[0]);
        } 
        else 
        {                    /* Process cha */
            close(fds[0]);
            write(fds[1], message, MSG_SIZE);
            close(fds[1]);
	          wait(NULL);
        }
    } else {
        printf("fork() unsuccessfully\n");      // fork() return -1 nếu lỗi.
    }
    return 0;
}
```
