# Sử dụng nhiều Pipe để giao tiếp giữa nhiều tiến trình

**Yêu cầu:**

- Mở rộng bài tập trước để sử dụng nhiều pipe cho ba tiến trình.
- Tạo hai pipe: một cho giao tiếp từ tiến trình cha đến tiến trình con 1, và một từ tiến trình con 1 đến tiến trình con 2.
- tiến trình cha gửi một thông điệp đến tiến trình con 1.
- tiến trình con 1 sẽ nhận thông điệp, thay đổi nó (ví dụ: thêm một chuỗi vào cuối), và gửi đến tiến trình con 2.
- tiến trình con 2 sẽ nhận thông điệp và in ra màn hình.

**Gợi ý:** Sử dụng nhiều pipe và đảm bảo rằng các tiến trình không bị chặn khi đọc và ghi dữ liệu.

**Code**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MSG_SIZE 256

int main() {
    int pipe_1[2], pipe_2[2];
    pid_t child_pid1, child_pid2;

    char *message_par_to_child_1 = "I'm parent, HELLO child 1";
    char *message_child_1_to_child_2 = "\nI'm child 1, Hello child 2";

    char pipe_buff_1[MSG_SIZE];
    char pipe_buff_2[MSG_SIZE];

    if (pipe(pipe_1) == -1 || pipe(pipe_2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    child_pid1 = fork();
    if (child_pid1 < 0) {
        perror("fork child 1");
        exit(EXIT_FAILURE);
    }

    if (child_pid1 == 0) {
        // === CHILD 1 ===
        close(pipe_1[1]); // Không ghi pipe_1
        close(pipe_2[0]); // Không đọc pipe_2

        read(pipe_1[0], pipe_buff_1, MSG_SIZE);
        printf("Child 1 received: %s\n", pipe_buff_1);

        snprintf(pipe_buff_2, MSG_SIZE, "%s%s", pipe_buff_1, message_child_1_to_child_2);

        write(pipe_2[1], pipe_buff_2, strlen(pipe_buff_2) + 1); // +1 để gửi cả '\0'

        close(pipe_1[0]);
        close(pipe_2[1]);
        exit(0);
    }

    // Only parent forks child 2
    child_pid2 = fork();
    if (child_pid2 < 0) {
        perror("fork child 2");
        exit(EXIT_FAILURE);
    }

    if (child_pid2 == 0) {
        // === CHILD 2 ===
        close(pipe_1[0]);
        close(pipe_1[1]);
        close(pipe_2[1]); // Không ghi

        read(pipe_2[0], pipe_buff_2, MSG_SIZE);
        printf("Child 2 received:\n%s\n", pipe_buff_2);

        close(pipe_2[0]);
        exit(0);
    }

    // === PARENT ===
    close(pipe_1[0]); // Không đọc
    close(pipe_2[0]);
    close(pipe_2[1]); // Không dùng

    write(pipe_1[1], message_par_to_child_1, strlen(message_par_to_child_1) + 1);
    close(pipe_1[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}
```

**Output**

```sh
Child 1 received: I'm parent, HELLO child 1
Child 2 received:
I'm parent, HELLO child 1
I'm child 1, Hello child 2
```
