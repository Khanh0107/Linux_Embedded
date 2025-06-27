# Question : Sử dụng exec và Biến Môi Trường
Mô tả: Viết một chương trình trong đó tiến trình con sẽ thay thế chương trình đang chạy bằng một chương trình khác, ví dụ ls hoặc date, sử dụng hàm execlp() hoặc execvp().

**Yêu cầu:**

- Truyền các biến môi trường vào chương trình và thực hiện các công việc khác nhau (ví dụ biến môi trường được truyền vào là 1 thì thực hiện lệnh ls, nếu là 2 thì thực hiện lệnh date).

- Giải thích điều gì xảy ra với tiến trình ban đầu sau khi exec được gọi

## Code
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    char *action = getenv("ACTION");
    // Tạo process con
    pid = fork();

    if (pid < 0) 
    {
        // fork thất bại
        perror("fork failed");
        return 1;
    } 
    else if (pid == 0) 
    {
        if (action[0] == '1') // Tiến trình con
        {
            printf("Child process: My PID is %d\n", getpid());
            execlp("ls", "ls", "-l", NULL);  // Thực thi lệnh ls -l
        }
        else if (action[0] == '2')
        {
            printf("Child process: My PID is %d\n", getpid());
            execlp("date", "date", NULL);    // Thực thi lệnh date
        }
        exit(1);
    }
    else 
    {
        // Tiến trình cha
        wait(NULL);
        printf("Parent process: Child PID is %d\n", pid);
    }

    return 0;
}
```

## Input 
``` sh
gcc -o test main.c
ACTION=2 ./test
```

## Output
``` sh
Child process: My PID is 3876
Thứ sáu, 27 Tháng 6 năm 2025 21:41:03 +07
Parent process: Child PID is 3876
```
