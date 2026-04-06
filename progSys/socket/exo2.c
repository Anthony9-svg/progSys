#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 12345
#define MAX_LIGNE 256
#define MAX_CLIENTS 100
#define FICHIER_AUTORIS "add_autoris.txt"

// Structure pour stocker les IP autorisées et noms
typedef struct {
    char ip[50];
    char nom[50];
} ClientAutorise;

// Charger les IP et noms depuis le fichier
int charger_autorisations(ClientAutorise **tab) {
    FILE *fp = fopen(FICHIER_AUTORIS, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    ClientAutorise *clients = NULL;
    int nb = 0;
    char ip[50], nom[50];

    while (fscanf(fp, "%49s %49s", ip, nom) == 2) {
        clients = realloc(clients, (nb+1) * sizeof(ClientAutorise));
        strcpy(clients[nb].ip, ip);
        strcpy(clients[nb].nom, nom);
        nb++;
    }

    fclose(fp);
    *tab = clients;
    return nb;
}

// Chercher le nom associé à une IP
const char* chercher_nom(ClientAutorise *tab, int taille, const char *ip) {
    for (int i = 0; i < taille; i++) {
        if (strcmp(tab[i].ip, ip) == 0)
            return tab[i].nom;
    }
    return NULL;
}

int main() {
    ClientAutorise *autorises;
    int nbAutorises = charger_autorisations(&autorises);
    if (nbAutorises <= 0) {
        fprintf(stderr, "Erreur chargement autorisations\n");
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind"); return 1;
    }

    listen(sockfd, 5);
    printf("Serveur en écoute sur le port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
        if (client < 0) { perror("accept"); continue; }

        char ip_client[50];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip_client, sizeof(ip_client));
        printf("Connexion de %s\n", ip_client);

        const char *nom_client = chercher_nom(autorises, nbAutorises, ip_client);
        if (!nom_client) {
            write(client, "Acces refuse: IP non autorisee\n", 31);
            close(client);
            printf("Connexion refusee pour %s\n", ip_client);
            continue;
        }

        // Envoi du message de bienvenue
        char message[MAX_LIGNE];
        snprintf(message, MAX_LIGNE, "Bienvenue %s !\n", nom_client);
        write(client, message, strlen(message));

        // Dialogue simple (comme exemple)
        write(client, "Vous etes connecte. Envoyez un message :\n", 43);
        int n = read(client, message, MAX_LIGNE);
        if (n > 0) {
            message[n] = '\0';
            snprintf(message, MAX_LIGNE, "Vous avez envoye : %s\n", message);
            write(client, message, strlen(message));
        }

        close(client);
    }

    free(autorises);
    close(sockfd);
    return 0;
}