#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

void* fonction(void* arg)
{
    int* tableau=malloc(99*sizeof(int));
    for (int i=0;i<99;i++)
        tableau[i]=rand()%100+1;
    pthread_exit(tableau);
}
int main()
{
    srand(time(NULL));
    pthread_t thread;
    void* tableau;
    int* tab;
    pthread_create(&thread,NULL,fonction,NULL);
    pthread_join(thread,&tableau);
    tab=(int*)tableau;
    for(int i=0;i<99;i++)
        printf("%d ",tab[i]);
    return 0;
}