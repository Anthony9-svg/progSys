#include <pthread.h>
#include <stdio.h>

void* fonction(void* arg)
{
    int* tableau=(int*)arg;
    for (int i=0;i<10;i++)
    {
        printf("%d ",tableau[i]);
    }
    pthread_exit(NULL);
}
int main()
{
    pthread_t thread;
    int tableau[10]={1,2,3,4,5,6,7,8,9,10};
    pthread_create(&thread,NULL,fonction,(void*)tableau);
    pthread_join(thread,NULL);
    return 0;
}