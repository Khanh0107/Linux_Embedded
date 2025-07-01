# Bắt Tín hiệu SIGINT

**Yêu cầu:** 

Viết một chương trình bắt tín hiệu SIGINT (được gửi khi nhấn Ctrl+C) và in ra thông báo khi nhận được tín hiệu này.

Sử dụng hàm signal() để đăng ký một hàm xử lý cho tín hiệu SIGINT.

Trong hàm xử lý, in ra thông báo "SIGINT received" mỗi khi nhận được tín hiệu SIGINT.

Chương trình sẽ tiếp tục chạy cho đến khi nhận được SIGINT lần thứ 3, sau đó kết thúc.

**Gợi ý:** 

Sử dụng một biến đếm toàn cục để đếm số lần tín hiệu SIGINT được nhận và kiểm tra biến này trong hàm xử lý.

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigint_count = 0;  // Biến đếm số lần nhận SIGINT

void handle_sigint(int sig) {
    sigint_count++;
    printf("SIGINT received (%d)\n", sigint_count);

    if (sigint_count >= 3) {
        printf("received SIGINT 3 times.\n");
        exit(0);  // Thoát chương trình
    }
}

int main() {
    // Đăng ký hàm xử lý SIGINT
    signal(SIGINT, handle_sigint);

    printf("Chương trình đang chạy. Nhấn Ctrl+C để gửi tín hiệu SIGINT.\n");

    // Vòng lặp vô hạn để chương trình tiếp tục chạy
    while (1) {
        
    }

    return 0;
}
```
