
# Sử dụng Tín hiệu SIGALRM để Tạo Timer

**Yêu cầu:**

Viết chương trình tạo một bộ đếm thời gian bằng cách sử dụng tín hiệu SIGALRM.

Sử dụng hàm alarm() để đặt thời gian kích hoạt SIGALRM mỗi giây.

Khi nhận tín hiệu SIGALRM, chương trình sẽ tăng biến đếm và in ra dòng "Timer: <giây> seconds".

Dừng chương trình sau khi đếm đến 10 giây.

**Gợi ý:**

Sử dụng hàm signal() để đăng ký xử lý tín hiệu SIGALRM, và đặt alarm(1) để bộ đếm lặp lại hàng giây.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int counter = 0;

void timer_handler(int sig) {
    counter++;
    printf("Timer: %d seconds\n", counter);

    if (counter < 10) {
        alarm(1); // tiếp tục đặt báo thức sau 1 giây nữa
    } else {
        printf("⏹ Đếm đủ 10 giây. Kết thúc chương trình.\n");
        exit(0);
    }
}

int main() {
    // Đăng ký hàm xử lý tín hiệu SIGALRM
    signal(SIGALRM, timer_handler);

    // Bắt đầu bằng cách đặt báo thức sau 1 giây
    alarm(1);

    // Chờ tín hiệu liên tục (giữ chương trình không thoát)
    while (1) {
        // pause(); // tạm dừng tiến trình cho đến khi nhận tín hiệu
    }

    return 0;
}
```

**Output**

```sh
Timer: 1 seconds
Timer: 2 seconds
Timer: 3 seconds
Timer: 4 seconds
Timer: 5 seconds
Timer: 6 seconds
Timer: 7 seconds
Timer: 8 seconds
Timer: 9 seconds
Timer: 10 seconds
⏹ Đếm đủ 10 giây. Kết thúc chương trình.
```
