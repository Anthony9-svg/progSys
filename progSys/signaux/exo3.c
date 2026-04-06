#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define N 5

pid_t fils[N];

void handler_fils(int sig)
{
    printf("Fils %d : je vais mourir...\n", getpid());
    fflush(stdout);
    exit(0);
}

void handler_pere(int sig)
{
    printf("\nPere %d tue\n", getpid());

    for (int i = 0; i < N; i++)
    {
        kill(fils[i], SIGTERM);
    }

    while (wait(NULL) > 0);

    exit(0);
}

int main()
{
    signal(SIGINT, handler_pere);

    for (int i = 0; i < N; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            signal(SIGTERM, handler_fils);

            while (1)
            {
                sleep(1);
            }
        }

        fils[i] = pid;
    }

    while (1)
    {
        int choix, num;

        printf("\n1. Endormir un fils\n");
        printf("2. Reveiller un fils\n");
        printf("3. Terminer un fils\n");
        printf("Choix : ");
        scanf("%d", &choix);

        printf("Numero du fils (0-4) : ");
        scanf("%d", &num);

        if (num < 0 || num >= N)
        {
            printf("Numero invalide\n");
            continue;
        }

        if (choix == 1)
        {
            kill(fils[num], SIGSTOP);
            printf("Fils %d endormi\n", fils[num]);
        }
        else if (choix == 2)
        {
            kill(fils[num], SIGCONT);
            printf("Fils %d reveille\n", fils[num]);
        }
        else if (choix == 3)
        {
            kill(fils[num], SIGTERM);
            printf("Signal de terminaison envoye\n");
        }
        else
        {
            printf("Choix invalide\n");
        }
    }

    return 0;
}