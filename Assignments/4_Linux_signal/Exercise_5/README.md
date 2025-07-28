## Question

Viết một chương trình có thể bỏ qua tín hiệu SIGTSTP (thường là Ctrl+Z).
- Sử dụng signal() hoặc sigaction() để bắt và bỏ qua tín hiệu SIGTSTP.
- Khi nhận tín hiệu SIGTSTP, chương trình sẽ không dừng mà tiếp tục chạy và in thông báo "SIGTSTP ignored".

**Gợi ý:** Đảm bảo chương trình sẽ tiếp tục chạy thay vì bị dừng khi nhấn Ctrl+Z.

**Câu hỏi:** Điều gì xảy ra nếu không xử lý tín hiệu SIGTSTP và người dùng nhấn Ctrl+Z?

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigint_count = 0;  // Biến đếm số lần nhận SIGINT

void handle_sigstop(int sig) {
    printf("SIGTSTP ignored\n");
    // exit(0);  // Thoát chương trình

}

int main() {
    // Đăng ký hàm xử lý SIGINT
    signal(SIGTSTP, handle_sigstop);

    printf("Chương trình đang chạy. Nhấn Ctrl+Zđể gửi tín hiệu SIGSTOP.\n");

    // Vòng lặp vô hạn để chương trình tiếp tục chạy
    while (1) {
        
    }

    return 0;
}
```

**Output**

```sh
Chương trình đang chạy. Nhấn Ctrl+Zđể gửi tín hiệu SIGSTOP.
^ZSIGTSTP ignored
```


BT5. 

Chương trình chờ tín hiệu kết hợp chờ người dùng nhập liệu
Mục tiêu: Viết một chương trình có thể nhận tín hiệu đồng thời cho phép người dùng nhập liệu từ bàn phím.
Hướng dẫn:
Dùng select() hoặc poll() để cho phép chương trình xử lý tín hiệu mà không làm gián đoạn khi nhận dữ liệu nhập từ bàn phím.
Khi nhận tín hiệu SIGINT, in ra thông báo “SIGINT received.”
Nếu nhận tín hiệu SIGTERM, thoát chương trình.
Gợi ý: Kết hợp giữa việc bắt tín hiệu và đọc nhập liệu để chương trình linh hoạt hơn.
