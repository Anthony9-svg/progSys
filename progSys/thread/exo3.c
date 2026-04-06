#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct TypeTableau
{
    int* tab;
    int nbr;
    int x;
}
TypeTableau;

void* initialisation(void* arg)
{

    TypeTableau* tableau=malloc(sizeof(TypeTableau));
    int nbr=*(int*)arg;
    tableau->tab=malloc(sizeof(int)*nbr);
    for (int i=0;i<nbr;i++)
    {
         tableau->tab[i]=rand()%100+1;
    }

    pthread_exit((void*)tableau);

}

void* recherche(void* arg)
{
  ;
    TypeTableau tableau=*(TypeTableau*)arg;
    int* resultat=malloc(sizeof(int));
    *resultat=0;
    for (int i=0;i<tableau.nbr;i++)
    {
        if(tableau.tab[i]==tableau.x)
        {
            *resultat=1;
            pthread_exit(resultat);
        }
    }

    pthread_exit((void*)resultat);
}

int main()
{
    srand(time(NULL));
    pthread_t thread1,thread2;
    int nbr=rand()%10+1;
    int nombre;
    TypeTableau tableau;
    void* arg1;
    void* arg2;
    int resultat;
    pthread_create(&thread1,NULL,initialisation,(void*)&nbr);
    printf("Donnez le nombre a rechercher: ");
    scanf("%d",&nombre);
    pthread_join(thread1,&arg1);
    tableau=*(TypeTableau*)arg1;
    tableau.x=nombre;
    pthread_create(&thread2,NULL,recherche,(void*)&tableau);
    pthread_join(thread2,&arg2);
    resultat=*(int*)arg2;
    if (resultat==1)
    {
        printf("Le nombre %d est dans le tableau\n",nombre);
    }
    else 
        printf("Le nombre %d n'est pas dans le tableau\n",nombre);
    return 0;
}