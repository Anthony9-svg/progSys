#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 12345
#define MAX_LIGNE 256
#define MAX_PRODUITS 100

typedef struct {
    char nom[50];
    float prix;
} Produit;

// 1)
int chargerProduits(const char *fichier, Produit **tab) {
    FILE *fp = fopen(fichier, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    int taille = 0;
    Produit *produits = NULL;
    char nom[50];
    float prix;

    while (fscanf(fp, "%49s %f", nom, &prix) == 2) {
        produits = realloc(produits, (taille + 1) * sizeof(Produit));
        strcpy(produits[taille].nom, nom);
        produits[taille].prix = prix;
        taille++;
    }

    fclose(fp);
    *tab = produits;
    return taille;
}

//recherche d' un produit par nom
Produit* chercherProduit(Produit *tab, int taille, const char *nom) {
    for (int i = 0; i < taille; i++) {
        if (strcmp(tab[i].nom, nom) == 0)
            return &tab[i];
    }
    return NULL;
}

// generation d'un nom de fichier unique basé sur la date et l'heure
void nomFichierUnique(char *buffer, size_t taille) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, taille, "commande_%04d%02d%02d_%02d%02d%02d.txt",
             tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
}

int main() {
    Produit *produits;
    int nbProduits = chargerProduits("produits.txt", &produits);
    if (nbProduits <= 0) {
        fprintf(stderr, "Erreur chargement produits\n");
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
        int client = accept(sockfd, NULL, NULL);
        if (client < 0) { perror("accept"); continue; }

        char buffer[MAX_LIGNE];
        char nomClient[50];
        float totalCommande = 0;

        write(client, "Nom du client : ", 16);
        int n = read(client, buffer, MAX_LIGNE);
        buffer[n] = '\0';
        buffer[strcspn(buffer, "\n")] = 0; // enlever \n
        strcpy(nomClient, buffer);

        write(client, "Entrez vos produits (format: produit quantite), fin par END\n", 61);

        FILE *fichierCommande;
        char nomFichier[64];
        nomFichierUnique(nomFichier, sizeof(nomFichier));
        fichierCommande = fopen(nomFichier, "w");
        fprintf(fichierCommande, "Client: %s\n", nomClient);
        fprintf(fichierCommande, "Commande:\n");

        while (1) {
            n = read(client, buffer, MAX_LIGNE);
            buffer[n] = '\0';
            buffer[strcspn(buffer, "\n")] = 0;

            if (strcmp(buffer, "END") == 0) break;

            char prodNom[50];
            int quantite;
            if (sscanf(buffer, "%49s %d", prodNom, &quantite) != 2) {
                write(client, "Format invalide. Essayez: produit quantite\n", 43);
                continue;
            }

            Produit *p = chercherProduit(produits, nbProduits, prodNom);
            if (!p) {
                write(client, "Produit non trouvé\n", 19);
                continue;
            }

            float prixTotal = p->prix * quantite;
            totalCommande += prixTotal;

            snprintf(buffer, MAX_LIGNE, "%s : %d x %.2f = %.2f\n", p->nom, quantite, p->prix, prixTotal);
            write(client, buffer, strlen(buffer));

            fprintf(fichierCommande, "%s : %d x %.2f = %.2f\n", p->nom, quantite, p->prix, prixTotal);
        }

        snprintf(buffer, MAX_LIGNE, "Total commande: %.2f\n", totalCommande);
        write(client, buffer, strlen(buffer));
        fprintf(fichierCommande, "Total commande: %.2f\n", totalCommande);
        fclose(fichierCommande);

        write(client, "Commande enregistrée.\n", 24);
        close(client);
    }

    free(produits);
    close(sockfd);
    return 0;
}