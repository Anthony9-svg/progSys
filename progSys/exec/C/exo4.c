#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    char* input=malloc(10);
    pid_t pid;
    int a;
    while (1)
    {
        pid=fork();

        if (pid==0)
        {
            printf("Repertoire(Entrez \"stop\" pour quitter): ");
            scanf("%s",input);

            if (strcmp(input,"stop")==0)        
                exit(3);      
            else
            {
                printf("\n");
                execl("/usr/bin/cp","cp","-r",".",input,NULL);
            }
        }
        if (pid!=0)
        {
            wait(&a);
            if (WEXITSTATUS(a)==3)
                exit(0);
        }
    }
}
