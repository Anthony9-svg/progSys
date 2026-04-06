#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int tube[2];
    pipe(tube);
    char desc[2];
    pid_t pid=fork();
    if (pid==0)
    {
        close(tube[1]);
        sprintf(desc,"%d",tube[0]);
        setenv("DESC",desc,1);
        char* args[]={"./fils.exe",NULL};
        execv("./fils.exe",args);
    }
    else
    {
        char* message="Bonjour fils";     
        close(tube[0]);
        write(tube[1],message,sizeof(message));
        close(tube[1]);
    }
}