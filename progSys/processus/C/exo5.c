#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    srand(time(NULL));
    int time1=(rand()%10)+1;
    int time2=(rand()%10)+1;
    pid_t pid1=fork();
    if (pid1==0)
    {
        sleep(time1);
        printf("Je suis le fils 1 qui a duré %d secondes\n",time1);

    }
    else
    {
        pid_t pid2=fork();
        if (pid2==0)
        {
                sleep(time2);
                printf("Je suis le fils 2 qui a duré %d secondes\n",time2); 
        }
        else
        {
            if (time2>time1)
            {
                waitpid(pid2,NULL,0);
                waitpid(pid1,NULL,0);
                return 0;
            }
            else
            {
                waitpid(pid1,NULL,0);
                waitpid(pid2,NULL,0);
                return 0;
            }
        }
    }
}