**Question:** Đồng Bộ Hóa Threads với Mutex

Viết một chương trình sử dụng pthread và mutex để thực hiện các thao tác sau:

Tạo một biến toàn cục counter khởi tạo bằng 0.

Tạo ba threads, mỗi thread tăng giá trị của counter lên 1, 1.000.000 lần.

Sử dụng mutex để đảm bảo rằng việc tăng counter được thực hiện an toàn.

In ra giá trị cuối cùng của counter khi tất cả các threads hoàn thành.

Hỏi: Tại sao cần mutex trong bài này? Điều gì xảy ra nếu bỏ mutex?

**Gợi ý:** Sử dụng pthread_mutex_lock và pthread_mutex_unlock để khóa và mở khóa mutex khi truy cập vào counter.

Code

```c
/* mutex_demo.c: increment a shared counter safely with a mutex */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS   3
#define NUM_INCREMENTS 1000000

long long counter = 0;                  /* shared resource            */
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg)
{
    for (int i = 0; i < NUM_INCREMENTS; ++i) {
        /* --- CRITICAL SECTION START --- */
        pthread_mutex_lock(&counter_lock);   // 1️⃣ LOCK
        counter++;                           // update shared data
        pthread_mutex_unlock(&counter_lock); // 2️⃣ UNLOCK
        /* ---  CRITICAL SECTION END  --- */
    }
    return NULL;
}

int main(void)
{
    pthread_t tids[NUM_THREADS];

    /* create threads */
    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_create(&tids[i], NULL, worker, NULL);

    /* wait for threads to finish */
    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(tids[i], NULL);

    printf("Expected value: %lld\n", (long long)NUM_THREADS * NUM_INCREMENTS);
    printf("Final counter : %lld\n", counter);

    pthread_mutex_destroy(&counter_lock);
    return 0;
}
```

**Output**

```sh
Expected value: 3000000
Final counter : 3000000
```
