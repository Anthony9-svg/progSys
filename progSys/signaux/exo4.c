#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

int *tab;
int n;
int i;
jmp_buf env;

void handler(int sig)
{
    printf("Erreur de segmentation : indice invalide\n");
    longjmp(env, 1);
}

int main()
{
    signal(SIGSEGV, handler);

    printf("Nombre d'elements : ");
    scanf("%d", &n);

    tab = malloc(n * sizeof(int));

    for (int j = 0; j < n; j++)
    {
        printf("tab[%d] = ", j);
        scanf("%d", &tab[j]);
    }

    setjmp(env);

    while (1)
    {
        printf("Indice i : ");
        scanf("%d", &i);

        if (i < 0 || i >= n)
        {
            raise(SIGSEGV);
        }

        printf("Valeur = %d\n", tab[i]);
        break;
    }

    free(tab);
    return 0;
}