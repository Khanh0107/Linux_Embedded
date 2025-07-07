**Question:** Sử Dụng Condition Variables

Viết một chương trình có hai threads: producer và consumer như sau:

Producer: Tạo ra một số ngẫu nhiên từ 1 đến 10 và lưu vào một biến toàn cục data.

Consumer: Đợi producer cung cấp dữ liệu, sau đó đọc và in ra dữ liệu đó.

Sử dụng pthread_cond_wait và pthread_cond_signal để đồng bộ hóa giữa producer và consumer, đảm bảo rằng consumer chỉ đọc dữ liệu khi producer đã cung cấp xong.

Lặp lại quá trình trên 10 lần và in ra tất cả các giá trị do consumer đọc được.

**Gợi ý:** Sử dụng pthread_cond_wait để cho consumer đợi cho đến khi có tín hiệu từ producer rằng dữ liệu đã sẵn sàng.

**Code**

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid_producer;
pthread_t tid_consumer;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int buffer = 0;         // Biến dùng chung
int condition = 0;      // 0: buffer trống, 1: buffer đầy

void *producer(void *arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        while (condition == 1) {
            pthread_cond_wait(&cond, &mutex);
        }

        buffer = rand() % 10;
        printf("Producer: %d\n", buffer);
        condition = 1;

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        while (condition == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("\tConsumer: %d\n", buffer);
        condition = 0;

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {

    pthread_create(&tid_producer, NULL, producer, NULL);
    pthread_create(&tid_consumer, NULL, consumer, NULL);

    pthread_join(tid_producer, NULL);
    pthread_join(tid_consumer, NULL);

    return EXIT_SUCCESS;
}
```

**Output**

```sh
Producer: 3
	Consumer: 3
Producer: 6
	Consumer: 6
Producer: 7
	Consumer: 7
Producer: 5
	Consumer: 5
Producer: 3
	Consumer: 3
Producer: 5
	Consumer: 5
Producer: 6
	Consumer: 6
Producer: 2
	Consumer: 2
Producer: 9
	Consumer: 9
Producer: 1
	Consumer: 1
```
