#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main()
{   
    int num=1;
    for (int i=0;i<5;i++)
    {
        pid_t pid=fork();
        if (pid==0)
        {
            printf("Je suis le fils %d\n",num);
            break;
        }
        num=num+1;
    }
}