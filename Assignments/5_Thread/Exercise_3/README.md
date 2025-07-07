**Question**

Viết một chương trình sử dụng thư viện pthread.h để thực hiện các thao tác sau:

Tạo hai threads. Mỗi thread sẽ in ra một chuỗi thông báo kèm theo ID của thread (ví dụ: "Thread 1: Hello from thread").

Sử dụng hàm pthread_join để đợi cả hai threads hoàn thành trước khi chương trình kết thúc.

Sử dụng pthread_create và pthread_join như thế nào? Khi nào thread kết thúc?

**Gợi ý:** Sử dụng hàm pthread_create để tạo threads và pthread_join để chờ các threads hoàn thành.

**Code**

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Hàm mà mỗi thread sẽ thực hiện
void* thread_function(void* arg) {
    int thread_num = *(int*)arg;  // Ép kiểu lại và lấy số thứ tự thread
    pthread_t tid = pthread_self();  // Lấy thread ID

    printf("Thread %d: Hello from thread (ID: %lu)\n", thread_num, (unsigned long)tid);

    pthread_exit(NULL);  // Kết thúc thread (tùy chọn, có thể bỏ)
}

int main() {
    pthread_t thread1, thread2;
    int t1 = 1, t2 = 2;

    // Tạo 2 thread
    pthread_create(&thread1, NULL, thread_function, &t1);
    pthread_create(&thread2, NULL, thread_function, &t2);

    // Đợi cả 2 thread hoàn thành
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Main thread: All threads completed.\n");

    return 0;
}
```

**Output**

```sh
Thread 1: Hello from thread (ID: 140438206301952)
Thread 2: Hello from thread (ID: 140438197909248)
Main thread: All threads completed.
```


