**Question:** 

Viết một chương trình thực hiện tạo 2 threads. Cả hai threads này đều dùng chung một hàm handle. Kiểm tra nếu là thread1 đang thực hiện thì in ra thông điệp bất kì để thông báo. Nếu là thread2 thì truyền vào dữ liệu kiểu struct human được khởi tạo từ trước, với các thông tin: Họ tên, năm sinh, sdt, quê quán.

**Code**

```c
#include <pthread.h>
#include <stdio.h>

struct human {
    char name [20];
    int birthyear;
    char phone[15];
    char hometown[20];
};

typedef enum {THREAD1, THREAD2} thread_type;

typedef struct {
    thread_type ttype;
    struct human information;
} thread_arg;

void *handle(void *arg) {
    thread_arg * t_arg = arg;
    if (t_arg->ttype == THREAD1)
    {
        printf("I am thread 1\n");
    }
    else if (t_arg->ttype == THREAD2)
    {
        printf("Name is %s\n",t_arg->information.name);
        printf("Birth year is %d\n",t_arg->information.birthyear);
        printf("Phone is %s\n",t_arg->information.phone);
        printf("Hometown is %s\n",t_arg->information.hometown);
    }

    return NULL;
}


int main() {
    struct human person = {"Khanh", 2000, "0979294207", "Ninh_Binh"};
    struct human noperson =  { "", 0, "", "" };

    thread_arg arg1 = { THREAD1, noperson };
    thread_arg arg2 = { THREAD2, person };   

    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, handle, &arg1);
    pthread_create(&thread2, NULL, handle, &arg2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}
```

**Output**

```sh
I am thread 1
Name is Khanh
Birth year is 2000
Phone is 0979294207
Hometown is Ninh_Binh
```


