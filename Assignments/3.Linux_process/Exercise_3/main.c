#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])   /* Cấp phát stack frame cho hàm main() */
{
    /* code */
    pid_t child_pid;                /* Lưu trong stack frame của main() */
    int status, ret;
    int retVal;
    ret = fork();         
    if (ret == 0)
    {
            /* Process con */
        printf("Im the child process, my PID: %d\n", getpid());
        printf("Child process terminate after 5 seconds\n");
	    sleep(5);
	    exit(43);
    } 
    else if(ret > 0)   
    {                  /* Process cha */
        retVal = wait(&status);
        printf("\nIm the parent process, PID child process: %d\n", retVal);   
        if (WIFEXITED(status))
        {
               printf("Normally termination, status = %d\n", WEXITSTATUS(status));
        }       
    } 
    else 
    {
        printf("fork() unsuccessfully\n");      // fork() return -1 nếu lỗi.
    }
    return 0;
}
