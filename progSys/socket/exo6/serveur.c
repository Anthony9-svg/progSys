#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_CLIENTS 100
#define MAX_LIGNE 1024

typedef struct {
    int sock;
    struct sockaddr_in addr;
} Client;

Client clients[MAX_CLIENTS];
int nb_clients = 0;

pthread_mutex_t mutex_clients = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_ecriture = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_parole = PTHREAD_MUTEX_INITIALIZER;

void diffuser(const char *message, int emetteur) {
    pthread_mutex_lock(&mutex_ecriture);

    for (int i = 0; i < nb_clients; i++) {
        if (clients[i].sock != emetteur) {
            write(clients[i].sock, message, strlen(message));
        }
    }

    pthread_mutex_unlock(&mutex_ecriture);
}

void *gerer_client(void *arg) {
    Client *client = (Client *)arg;
    char buffer[MAX_LIGNE];
    char ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &client->addr.sin_addr, ip, sizeof(ip));

    write(client->sock, "Tapez 1 pour parler\n", 20);

    while (1) {
        int n = read(client->sock, buffer, MAX_LIGNE - 1);

        if (n <= 0)
            break;

        buffer[n] = '\0';

        if (strcmp(buffer, "1\n") == 0 || strcmp(buffer, "1") == 0) {
            pthread_mutex_lock(&mutex_parole);

            write(client->sock, "Vous pouvez parler : ", 21);

            n = read(client->sock, buffer, MAX_LIGNE - 1);

            if (n <= 0) {
                pthread_mutex_unlock(&mutex_parole);
                break;
            }

            buffer[n] = '\0';

            char msg[MAX_LIGNE + 100];
            sprintf(msg, "%s dit : %s", ip, buffer);

            diffuser(msg, client->sock);

            pthread_mutex_unlock(&mutex_parole);
        }
    }

    close(client->sock);
    pthread_exit(NULL);
}

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(sockfd, 10);

    printf("Serveur en écoute sur le port %d...\n", PORT);

    while (1) {
        Client nouveau;
        socklen_t len = sizeof(nouveau.addr);

        nouveau.sock = accept(
            sockfd,
            (struct sockaddr *)&nouveau.addr,
            &len
        );

        pthread_mutex_lock(&mutex_clients);

        clients[nb_clients] = nouveau;

        pthread_t tid;
        pthread_create(
            &tid,
            NULL,
            gerer_client,
            &clients[nb_clients]
        );

        nb_clients++;

        pthread_mutex_unlock(&mutex_clients);
    }

    close(sockfd);
    return 0;
}