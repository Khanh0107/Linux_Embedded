# Question: Đếm số kí tự qua Pipe

**Yêu cầu:** 

Viết một chương trình sử dụng pipe để đếm số ký tự trong một chuỗi.

Tạo một pipe và fork một tiến trình con.

Tiến trình cha sẽ gửi một chuỗi ký tự vào pipe.

Tiến trình con sẽ đọc chuỗi ký tự từ pipe và đếm số ký tự, sau đó in ra kết quả.

**Gợi ý:**

Sử dụng hàm strlen() để đếm số ký tự trong chuỗi đã đọc từ pipe.


```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MSG_SIZE    100

char * message = "123456789";

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
            printf("Message received: %s\nNumber of character: %ld", pipe_buff, strlen(pipe_buff));
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

**Output**

```sh
Message received: 123456789
Number of character: 9
```
