#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid=fork();
    if (pid==0)
        printf("Je suis le fils\n");
    else
    {
        waitpid(0,NULL,0);
        printf("Je suis le pere\n");
    }
    return 0;    
}