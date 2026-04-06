#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int num=0;

int main()
{
    for (int i=0;i<5;i++)
    {
        pid_t pid=fork();
        if (pid==0)
        {
            printf("Je suis le fils numero %d,pid=%d \n",i+1,getpid());
            break;
        } 
        else if (pid!=0)
        {
            waitpid(0,NULL,0);
            printf("Je suis le pere\n");
        }
    }
    return 0;
}