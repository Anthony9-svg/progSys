#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>


int main()
{
    char mot[10];
    char mot2[10];
    char* phrase=malloc(30);
    FILE* f;
    int i;
    int tube[2];
    int tube1[2];
    pipe(tube);
    pipe(tube1);
    pid_t pid=fork();
    if (pid==0)
    {

        printf("Donnez un mot: ");
        scanf("%s",mot);

        close(tube1[0]);
        write(tube1[1],mot,10);
        close(tube1[1]);

        close(tube[1]);
        read(tube[0],&i,sizeof(int));
        close(tube[0]);

        if (i==1)
        {
            printf("Le mot est dans le fichier\n");
        }
        else 
            printf("Le mot n'est pas dans le fichier\n");
        fclose(f);
    }
    else
    {
        int signal=0;
        f=fopen("file","r");
        close(tube1[1]);
        read(tube1[0],mot2,10);
        close(tube1[0]);
        while(fgets(phrase,30,f)!=NULL)
        {
            if (strstr(phrase,mot2)==0)
            {
                signal=1;
                break;
            }
        }
        close(tube[0]);
        write(tube[1],&signal,sizeof(int));
        close(tube[1]);
        wait(NULL);
    }
}