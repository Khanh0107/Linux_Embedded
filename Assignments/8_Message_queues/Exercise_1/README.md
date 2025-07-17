# Question: Gửi và nhận tin nhắn đơn giản

**Yêu cầu:**

Viết một chương trình sử dụng POSIX message queue để gửi và nhận một tin nhắn đơn giản.

Tạo một message queue bằng cách sử dụng hàm `mq_open()`.

Gửi một tin nhắn chứa một chuỗi ký tự từ tiến trình cha đến tiến trình con.

Tiến trình con sẽ nhận tin nhắn và in ra nội dung của nó.

**Gợi ý:**

Sử dụng cấu trúc mq_msg để định nghĩa tin nhắn.

Sử dụng các hàm `mq_send()` và `mq_receive()` để gửi và nhận tin nhắn.

Đừng quên đóng và xóa message queue sau khi sử dụng.

**Code**

```c
// sender_priority_attr.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define QUEUE_NAME  "/priority_queue"
#define MAX_SIZE    1024

int main() {
    mqd_t mq;
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    pid_t pid;
    char buffer[MAX_SIZE];
    unsigned int priority;
    // Tạo process con
    pid = fork();

    if (pid < 0) {
        // fork thất bại
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Tiến trình con

        mq = mq_open(QUEUE_NAME, O_RDONLY);
        if (mq == -1) {
            perror("mq_open");
            exit(1);
        }

        for (int i = 0; i < 3; i++) {
            if (mq_receive(mq, buffer, MAX_SIZE, &priority) == -1) {
                perror("mq_receive");
                exit(1);
            }
            printf("Received: '%s' (priority: %u)\n", buffer, priority);
        }

        mq_close(mq);
        mq_unlink(QUEUE_NAME);
    } else {
        // Tiến trình cha

        mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
        if (mq == -1) {
            perror("mq_open");
            exit(1);
        }

        mq_send(mq, "Message with priority 5", strlen("Message with priority 5") + 1, 5);
        mq_send(mq, "Message with priority 1", strlen("Message with priority 1") + 1, 1);
        mq_send(mq, "Message with priority 10", strlen("Message with priority 10") + 1, 10);

        printf("Messages sent with priorities 5, 1, and 10.\n");

        mq_close(mq);
        wait(NULL);
    }
    return 0;
}
```

**Output**

```sh
Messages sent with priorities 5, 1, and 10.
Received: 'Message with priority 10' (priority: 10)
Received: 'Message with priority 5' (priority: 5)
Received: 'Message with priority 1' (priority: 1)
```


