#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int iteration;
double u;

double f_1(double x)
{
    return((1.0/4.0)*(x-1)*(x-1));  
}
double f_2(double x)
{
    return ((1.0/6.0)*(x-2)*(x-2));
}

void* thread(void* arg)
{
    double(*f)(double x)=(double(*)(double))arg;
    while(iteration<20)
    {
        pthread_mutex_lock(&mutex);
        u=f(u);
        printf("u=%f ,iteration:%d\n",u,iteration);
        iteration++;
        pthread_mutex_unlock(&mutex);
        sleep(rand()%5+1);
    }
}
int main()
{
    pthread_t thread1,thread2;
    u=1;
    pthread_create(&thread1,NULL,thread,(void*)&f_1);
    pthread_create(&thread2,NULL,thread,(void*)&f_2);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
}