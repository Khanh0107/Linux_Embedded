#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    char *action = getenv("ACTION");
    // Tạo process con
    pid = fork();

    if (pid < 0) 
    {
        // fork thất bại
        perror("fork failed");
        return 1;
    } 
    else if (pid == 0) 
    {
        if (action[0] == '1') // Tiến trình con
        {
            printf("Child process: My PID is %d\n", getpid());
            execlp("ls", "ls", "-l", NULL);  // Thực thi lệnh ls -l
        }
        else if (action[0] == '2')
        {
            printf("Child process: My PID is %d\n", getpid());
            execlp("date", "date", NULL);    // Thực thi lệnh date
        }
        exit(1);
    }
    else 
    {
        // Tiến trình cha
        wait(NULL);
        printf("Parent process: Child PID is %d\n", pid);
    }

    return 0;
}
