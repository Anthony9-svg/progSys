#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    int x;
    pid_t pid;

    printf("Entrez l'entier : ");
    scanf("%d", &x);

    FILE *f = fopen("/tmp/entier.txt", "w");

    if (!f)
    {
        perror("fopen");
        return 1;
    }

    fprintf(f, "%d", x);
    fclose(f);

    printf("PID de affiche : ");
    scanf("%d", &pid);

    kill(pid, SIGUSR1);

    return 0;
}