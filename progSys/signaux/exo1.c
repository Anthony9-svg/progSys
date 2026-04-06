#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        long long i = 0;
        while (1)
        {
            i++;
            if (i % 100000000 == 0)
            {
                printf("Calcul en cours : %lld\n", i);
                fflush(stdout);
            }
        }
    }
    else
    {
        char choix;
        while (1)
        {
            printf("\nMenu :\n");
            printf("s : stopper le fils\n");
            printf("r : reprendre le fils\n");
            printf("q : quitter\n");
            printf("Choix : ");
            scanf(" %c", &choix);
            if (choix == 's')
            {
                kill(pid, SIGSTOP);
                printf("Fils endormi\n");
            }
            else if (choix == 'r')
            {
                kill(pid, SIGCONT);
                printf("Fils redemarre\n");
            }
            else if (choix == 'q')
            {
                kill(pid, SIGKILL);
                wait(NULL);
                printf("Fils tue\n");
                break;
            }
            else
            {
                printf("Choix invalide\n");
            }
        }
    }

    return 0;
}