#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid=fork();
    if (pid==0)
    {
        printf("Je suis le fils 1 avec le PID %d\n",getpid());
    }
    else
    {
        pid_t pid2=fork();
        if (pid2==0)
        {
            printf("Je suis le fils 2 avec le PID %d\n",getpid());
        }
        else
        {
            waitpid(0,NULL,0);
            printf("Je suis le père\n");
        }
    }
    return 0;
}