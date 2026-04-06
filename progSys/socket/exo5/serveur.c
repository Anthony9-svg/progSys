#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_LIGNE 1024

void relier_clients(int client1, int client2) {
    char buffer[MAX_LIGNE];
    int n;

    write(client1, "Vous etes connecte au client 2\n", 31);
    write(client2, "Vous etes connecte au client 1\n", 31);

    while (1) {
        // client 1 parle
        write(client1, "Saisissez votre message : ", 26);

        n = read(client1, buffer, MAX_LIGNE - 1);
        if (n <= 0) break;

        buffer[n] = '\0';

        char msg[MAX_LIGNE + 50];
        sprintf(msg, "L'autre client dit : %sSaisissez la réponse : ", buffer);

        write(client2, msg, strlen(msg));

        // client 2 répond
        n = read(client2, buffer, MAX_LIGNE - 1);
        if (n <= 0) break;

        buffer[n] = '\0';

        sprintf(msg, "L'autre client dit : %sSaisissez la réponse : ", buffer);

        write(client1, msg, strlen(msg));
    }

    close(client1);
    close(client2);
}

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(sockfd, 5);

    printf("Serveur en écoute sur le port %d...\n", PORT);

    while (1) {
        printf("En attente du client 1...\n");
        int client1 = accept(sockfd, NULL, NULL);

        write(client1, "En attente du second client...\n", 31);

        printf("En attente du client 2...\n");
        int client2 = accept(sockfd, NULL, NULL);

        relier_clients(client1, client2);
    }

    close(sockfd);
    return 0;
}