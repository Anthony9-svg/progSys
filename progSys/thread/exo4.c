#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define N1 5  
#define N2 10 


void* fonction(void* arg)
{
    int taille = *(int*)arg;
    int* tableau = malloc(taille * sizeof(int));
    for (int i = 0; i < taille; i++)
        tableau[i] = rand() % 100;
    pthread_exit((void*)tableau);
}


void* inclusion(void* arg)
{
    int** tab = (int**)arg;
    int* T1 = tab[0];
    int n1 = *(int*)tab[1];
    int* T2 = tab[2];
    int n2 = *(int*)tab[3];

    int* res = malloc(sizeof(int));
    *res = 1;

    for (int i = 0; i < n1; i++) {
        int trouve = 0;
        for (int j = 0; j < n2; j++) {
            if (T1[i] == T2[j]) {
                trouve = 1;
                break;
            }
        }
        if (!trouve) {
            *res = 0;
            break;
        }
    }
    pthread_exit(res);
}


void* annulation(void* arg)
{
    char c;
    printf("Appuyez sur 'A' pour annuler le programme...\n");
    c = getchar();
    if (c == 'A' || c == 'a') {
        printf("Programme annulé par l'utilisateur !\n");
        exit(0);
    }
    pthread_exit(NULL);
}

int main()
{
    srand(time(NULL));

    pthread_t th1, th2, th_incl, th_cancel;
    void* T1_void;
    void* T2_void;

    int n1 = N1;
    int n2 = N2;


    pthread_create(&th_cancel, NULL, annulation, NULL);

    pthread_create(&th1, NULL, fonction, &n1);
    pthread_create(&th2, NULL, fonction, &n2);


    pthread_join(th1, &T1_void);
    pthread_join(th2, &T2_void);

    int* T1 = (int*)T1_void;
    int* T2 = (int*)T2_void;


    void* args[4];
    args[0] = T1;
    args[1] = &n1;
    args[2] = T2;
    args[3] = &n2;

    void* res_void;
    pthread_create(&th_incl, NULL, inclusion, args);
    pthread_join(th_incl, &res_void);
    int res = *(int*)res_void;

    if (res)
        printf("T1 est inclus dans T2\n");
    else
        printf("T1 n'est pas inclus dans T2\n");

    pthread_cancel(th_cancel);
    pthread_join(th_cancel, NULL);

    return 0;
}