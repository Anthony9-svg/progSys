#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t recu = 0;

void handler(int sig)
{
    recu = 1;
}

int main()
{
    signal(SIGUSR1, handler);

    printf("PID de affiche : %d\n", getpid());
    printf("En attente du signal...\n");

    while (!recu)
    {
        sleep(1);
    }

    FILE *f = fopen("/tmp/entier.txt", "r");

    if (!f)
    {
        perror("fopen");
        return 1;
    }

    int x;
    fscanf(f, "%d", &x);
    fclose(f);

    printf("Entier reçu : %d\n", x);

    return 0;
}