#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    // Tạo process con
    pid = fork();

    if (pid < 0) {
        // fork thất bại
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Tiến trình con
        printf("Child process: My PID is %d\n", getpid());
    } else {
        // Tiến trình cha
        printf("Parent process: Child PID is %d\n", pid);
    }

    return 0;
}
