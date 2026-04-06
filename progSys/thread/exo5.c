#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


int compt = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int limite;


void* incrementer(void* arg) {
    while (1) {

        int pause = rand() % 5 + 1;
        sleep(pause);

   
        pthread_mutex_lock(&mutex);
        if (compt >= limite) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        compt++;
        printf("[Incrément] compteur = %d\n", compt);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}


void* afficher(void* arg) {
    while (1) {
        sleep(2);
        pthread_mutex_lock(&mutex);
        if (compt >= limite) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        printf("[Affichage] compteur = %d\n", compt);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage : %s <valeur_limite>\n", argv[0]);
        return 1;
    }

    limite = atoi(argv[1]);
    if (limite <= 0) {
        printf("La valeur limite doit être positive.\n");
        return 1;
    }

    srand(time(NULL));

    pthread_t th_inc, th_aff;


    pthread_create(&th_inc, NULL, incrementer, NULL);
    pthread_create(&th_aff, NULL, afficher, NULL);


    pthread_join(th_inc, NULL);
    pthread_join(th_aff, NULL);

    printf("Compteur final = %d\n", compt);
    return 0;
}