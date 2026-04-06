#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define N 100

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
double tab[N];


void* moyenne(void* arg)
{
    double* tab=(double*) arg;
    double copie[N];


    for(int i=0;i<100;i++)
        copie[i]=tab[i];

    for (int i=1;i<99;i++)
        {
            pthread_mutex_lock(&mutex);
            tab[i]=(copie[i-1]+copie[i]+copie[i+1])/3;
            pthread_mutex_unlock(&mutex);
            sleep(rand()%3+1);
        }
   
}
void* affichage(void* arg)
{
    double* tab=(double*) arg;
    
    int cmp=1;
    while (cmp<=99)
    {
        for (int i=0;i<100;i++)
        {
            pthread_mutex_lock(&mutex);
            printf("%f ",tab[i]);
            pthread_mutex_unlock(&mutex);
        }
        sleep(4);
        cmp++;
    }   
    sleep(4);
}
int main()
{
    srand(time(NULL));
    tab[0]=0;
    tab[99]=0;

    for(int i=1;i<99;i++)
    {
        tab[i]=rand()%100+1;
    }

    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1,NULL,moyenne,(void*)&tab[0]);
    pthread_create(&thread2,NULL,affichage,(void*)&tab[0]);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    return 0;

}